import numpy as np
import tvm
from tvm.script import tir as T

@tvm.script.ir_module
class MyModule:
    @T.prim_func
    def mm_relu(
        A: T.Buffer[(128, 128), "float32"],
        B: T.Buffer[(128, 128), "float32"],
        C: T.Buffer[(128, 128), "float32"],
    ):
        Y = T.alloc_buffer((128, 128), dtype="float32")
        for i, j, k in T.grid(128, 128, 128):
            with T.block("Y"):
                vi, vj, vk = T.axis.remap("SSR", [i, j, k])
                with T.init():
                    Y[vi, vj] = 0.0
                Y[vi, vj] += A[vi, vk] * B[vk, vj]
        for i, j in T.grid(128, 128):
            with T.block("C"):
                vi, vj = T.axis.remap("SS", [i, j])
                C[vi, vj] = T.max(Y[vi, vj], 0.0)

def optimize_mm_relu(A: np.ndarray, B: np.ndarray, target: str = "llvm") -> np.ndarray:
    """Compile and execute an optimized matrix multiplication + ReLU pipeline."""
    dtype = "float32"
    sch = tvm.tir.Schedule(MyModule)
    block_Y = sch.get_block("Y", func_name="mm_relu")
    i, j, k = sch.get_loops(block_Y)
    j0, j1 = sch.split(j, factors=[None, 4])
    sch.reorder(j0, k, j1)
    block_C = sch.get_block("C", "mm_relu")
    sch.reverse_compute_at(block_C, j0)
    sch.decompose_reduction(block_Y, k)

    rt_lib = tvm.build(sch.mod, target=target)
    f = rt_lib["mm_relu"]

    a_nd = tvm.nd.array(A.astype(dtype))
    b_nd = tvm.nd.array(B.astype(dtype))
    c_nd = tvm.nd.empty((128, 128), dtype=dtype)

    f(a_nd, b_nd, c_nd)
    return c_nd.numpy()
