python3 -m  pip install mlc-ai-nightly -f https://mlc.ai/wheels

import numpy as np
import tvm
from tvm.ir.module import 
IRModule
from tvm.script import tir as T
dtype = "float32"
a_np = np.random.rand(128, 128).astype(dtype)
b_np = np.random.rand(128, 128).astype(dtype)
# a @ b is equivalent to np.matmul(a, b)
c_mm_relu = np.maximum(a_np @ b_np, 0)

def lnumpy_mm_relu(A: np.ndarray, B: np.ndarray, C: np.ndarray):
    Y = np.empty((128, 128), dtype="float32")
    for i in range(128):
        for j in range(128):
            for k in range(128):
                if k == 0:
                    Y[i, j] = 0
                Y[i, j] = Y[i, j] + A[i, k] * B[k, j]
    for i in range(128):
        for j in range(128):
            C[i, j] = max(Y[i, j], 0)

c_np = np.empty((128, 128), dtype=dtype)
lnumpy_mm_relu(a_np, b_np, c_np)
np.testing.assert_allclose(c_mm_relu, c_np, rtol=1e-5)

@tvm.script.ir_module
class MyModule:
    @T.prim_func
    def mm_relu(A: T.Buffer((128, 128), "float32"),
                B: T.Buffer((128, 128), "float32"),
                C: T.Buffer((128, 128), "float32")):
        T.func_attr({"global_symbol": "mm_relu", "tir.noalias": True})
        Y = T.alloc_buffer((128, 128), dtype="float32")
        for i, j, k in T.grid(128, 128, 128):
            with T.block("Y"):
                vi = T.axis.spatial(128, i)
                vj = T.axis.spatial(128, j)
                vk = T.axis.reduce(128, k)
                with T.init():
                    Y[vi, vj] = T.float32(0)
                Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]
        for i, j in T.grid(128, 128):
            with T.block("C"):
                vi = T.axis.spatial(128, i)
                vj = T.axis.spatial(128, j)
                C[vi, vj] = T.max(Y[vi, vj], T.float32(0))

# TensorIR
def mm_relu(A: T.Buffer[(128, 128), "float32"],
            B: T.Buffer[(128, 128), "float32"],
            C: T.Buffer[(128, 128), "float32"]):
    ...
# numpy
def lnumpy_mm_relu(A: np.ndarray, B: np.ndarray, C: np.ndarray):
    ...

# TensorIR
Y = T.alloc_buffer((128, 128), dtype="float32")
# numpy
Y = np.empty((128, 128), dtype="float32")

# TensorIR
for i, j, k in T.grid(128, 128, 128):

# numpy
for i in range(128):
    for j in range(128):
        for k in range(128):

# TensorIR
with T.block("Y"):
    vi = T.axis.spatial(128, i)
    vj = T.axis.spatial(128, j)
    vk = T.axis.reduce(128, k)
    with T.init():
        Y[vi, vj] = T.float32(0)
    Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]

# coressponding numpy code
vi, vj, vk = i, j, k
if vk == 0:
    Y[vi, vj] = 0
Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]

@tvm.script.ir_module
class MyModuleWithAxisRemapSugar:
    @T.prim_func
    def mm_relu(A: T.Buffer((128, 128), "float32"),
                B: T.Buffer((128, 128), "float32"),
                C: T.Buffer((128, 128), "float32")):
        T.func_attr({"global_symbol": "mm_relu", "tir.noalias": True})
        Y = T.alloc_buffer((128, 128), dtype="float32")
        for i, j, k in T.grid(128, 128, 128):
            with T.block("Y"):
                vi, vj, vk = T.axis.remap("SSR", [i, j, k])
                with T.init():
                    Y[vi, vj] = T.float32(0)
                Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]
        for i, j in T.grid(128, 128):
            with T.block("C"):
                vi, vj = T.axis.remap("SS", [i, j])
                C[vi, vj] = T.max(Y[vi, vj], T.float32(0))

def lnumpy_mm_relu_v2(A: np.ndarray, B: np.ndarray, C: np.ndarray):
    Y = np.empty((128, 128), dtype="float32")
    for i in range(128):
        for j0 in range(32):
            for k in range(128):
                for j1 in range(4):
                    j = j0 * 4 + j1
                    if k == 0:
                        Y[i, j] = 0
                    Y[i, j] = Y[i, j] + A[i, k] * B[k, j]
    for i in range(128):
        for j in range(128):
            C[i, j] = max(Y[i, j], 0)

c_np = np.empty((128, 128), dtype=dtype)
lnumpy_mm_relu_v2(a_np, b_np, c_np)
np.testing.assert_allclose(c_mm_relu, c_np, rtol=1e-5)

@tvm.script.ir_module
class MyModuleWithTwoFunctions:
    @T.prim_func
    def mm(A: T.Buffer((128, 128), "float32"),
           B: T.Buffer((128, 128), "float32"),
           Y: T.Buffer((128, 128), "float32")):
        T.func_attr({"global_symbol": "mm", "tir.noalias": True})
        for i, j, k in T.grid(128, 128, 128):
            with T.block("Y"):
                vi, vj, vk = T.axis.remap("SSR", [i, j, k])
                with T.init():
                    Y[vi, vj] = T.float32(0)
                Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]

    @T.prim_func
    def relu(A: T.Buffer((128, 128), "float32"),
             B: T.Buffer((128, 128), "float32")):
        T.func_attr({"global_symbol": "relu", "tir.noalias": True})
        for i, j in T.grid(128, 128):
            with T.block("B"):
                vi, vj = T.axis.remap("SS", [i, j])
                B[vi, vj] = T.max(A[vi, vj], T.float32(0))


def lnumpy_mm_relu_v2(A: np.ndarray, B: np.ndarray, C: np.ndarray):
    Y = np.empty((128, 128), dtype="float32")
    for i in range(128):
        for j0 in range(32):
            for k in range(128):
                for j1 in range(4):
                    j = j0 * 4 + j1
                    if k == 0:
                        Y[i, j] = 0
                    Y[i, j] = Y[i, j] + A[i, k] * B[k, j]
    for i in range(128):
        for j in range(128):
            C[i, j] = max(Y[i, j], 0)

c_np = np.empty((128, 128), dtype=dtype)
lnumpy_mm_relu_v2(a_np, b_np, c_np)
np.testing.assert_allclose(c_mm_relu, c_np, rtol=1e-5)

import IPython

IPython.display.Code(MyModule.script(), language="python")
# from tvm.script import ir as I
# from tvm.script import tir as T

@I.ir_module
class Module:
    @T.prim_func
    def mm_relu(A: T.Buffer((128, 128), "float32"), B: T.Buffer((128, 128), "float32"), C: T.Buffer((128, 128), "float32")):
        T.func_attr({"tir.noalias": T.bool(True)})
        # with T.block("root"):
        Y = T.alloc_buffer((128, 128))
        for i, j, k in T.grid(128, 128, 128):
            with T.block("Y"):
                vi, vj, vk = T.axis.remap("SSR", [i, j, k])
                T.reads(A[vi, vk], B[vk, vj])
                T.writes(Y[vi, vj])
                with T.init():
                    Y[vi, vj] = T.float32(0.0)
                Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]
        for i, j in T.grid(128, 128):
            with T.block("C"):
                vi, vj = T.axis.remap("SS", [i, j])
                T.reads(Y[vi, vj])
                T.writes(C[vi, vj])
                C[vi, vj] = T.max(Y[vi, vj], T.float32(0.0))
Now we are ready to try out the code transformations, we begin by creating a Schedule helper class with the given MyModule as input.

sch = tvm.tir.Schedule(MyModuleWithAxisRemapSugar)
Copy to clipboard
Then we perform the following operations to obtain a reference to block Y and corresponding loops.

block_Y = sch.get_block("Y", func_name="mm_relu")
i, j, k = sch.get_loops(block_Y)
Copy to clipboard
Now we are ready to perform the transformations. The first transformation we will perform is to split the loop j into two loops, with the length of the inner loop to be 4. Note that the transformation is procedural, so if you accidentally execute the block twice, we will get an error that variable j no longer exists. If that happens, you can run again from the beginning (where sch get created).

j0, j1 = sch.split(j, factors=[None, 4])
Copy to clipboard
We can look at the result of the transformation, which is stored at sch.mod.

IPython.display.Code(sch.mod.script(), language="python")
Copy to clipboard
# from tvm.script import ir as I
# from tvm.script import tir as T

@I.ir_module
class Module:
    @T.prim_func
    def mm_relu(A: T.Buffer((128, 128), "float32"), B: T.Buffer((128, 128), "float32"), C: T.Buffer((128, 128), "float32")):
        T.func_attr({"tir.noalias": T.bool(True)})
        # with T.block("root"):
        Y = T.alloc_buffer((128, 128))
        for i, j_0, j_1, k in T.grid(128, 32, 4, 128):
            with T.block("Y"):
                vi = T.axis.spatial(128, i)
                vj = T.axis.spatial(128, j_0 * 4 + j_1)
                vk = T.axis.reduce(128, k)
                T.reads(A[vi, vk], B[vk, vj])
                T.writes(Y[vi, vj])
                with T.init():
                    Y[vi, vj] = T.float32(0.0)
                Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]
        for i, j in T.grid(128, 128):
            with T.block("C"):
                vi, vj = T.axis.remap("SS", [i, j])
                T.reads(Y[vi, vj])
                T.writes(C[vi, vj])
                C[vi, vj] = T.max(Y[vi, vj], T.float32(0.0))
After the first step of transformation, we created two additional loops, j_0 and j_1, with corresponding ranges 32 and 4. Our next step would be to reorder the two loops.

sch.reorder(j0, k, j1)
IPython.display.Code(sch.mod.script(), language="python")
Copy to clipboard
# from tvm.script import ir as I
# from tvm.script import tir as T

@I.ir_module
class Module:
    @T.prim_func
    def mm_relu(A: T.Buffer((128, 128), "float32"), B: T.Buffer((128, 128), "float32"), C: T.Buffer((128, 128), "float32")):
        T.func_attr({"tir.noalias": T.bool(True)})
        # with T.block("root"):
        Y = T.alloc_buffer((128, 128))
        for i, j_0, k, j_1 in T.grid(128, 32, 128, 4):
            with T.block("Y"):
                vi = T.axis.spatial(128, i)
                vj = T.axis.spatial(128, j_0 * 4 + j_1)
                vk = T.axis.reduce(128, k)
                T.reads(A[vi, vk], B[vk, vj])
                T.writes(Y[vi, vj])
                with T.init():
                    Y[vi, vj] = T.float32(0.0)
                Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]
        for i, j in T.grid(128, 128):
            with T.block("C"):
                vi, vj = T.axis.remap("SS", [i, j])
                T.reads(Y[vi, vj])
                T.writes(C[vi, vj])
                C[vi, vj] = T.max(Y[vi, vj], T.float32(0.0))
Now code after reordering closely resembles lnumpy_mm_relu_v2.

2.4.4.1. Getting to Another Variant

In this section, we are going to go ahead and do another two steps of transformations to get to another variant. First, we use a primitive called reverse_compute_at to move block C to an inner loop of Y.

block_C = sch.get_block("C", "mm_relu")
sch.reverse_compute_at(block_C, j0)
IPython.display.Code(sch.mod.script(), language="python")
Copy to clipboard
# from tvm.script import ir as I
# from tvm.script import tir as T

@I.ir_module
class Module:
    @T.prim_func
    def mm_relu(A: T.Buffer((128, 128), "float32"), B: T.Buffer((128, 128), "float32"), C: T.Buffer((128, 128), "float32")):
        T.func_attr({"tir.noalias": T.bool(True)})
        # with T.block("root"):
        Y = T.alloc_buffer((128, 128))
        for i, j_0 in T.grid(128, 32):
            for k, j_1 in T.grid(128, 4):
                with T.block("Y"):
                    vi = T.axis.spatial(128, i)
                    vj = T.axis.spatial(128, j_0 * 4 + j_1)
                    vk = T.axis.reduce(128, k)
                    T.reads(A[vi, vk], B[vk, vj])
                    T.writes(Y[vi, vj])
                    with T.init():
                        Y[vi, vj] = T.float32(0.0)
                    Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]
            for ax0 in range(4):
                with T.block("C"):
                    vi = T.axis.spatial(128, i)
                    vj = T.axis.spatial(128, j_0 * 4 + ax0)
                    T.reads(Y[vi, vj])
                    T.writes(C[vi, vj])
                    C[vi, vj] = T.max(Y[vi, vj], T.float32(0.0))
So far, we have kept the reduction initialization and update step together in a single block body. This combined form brings convenience for loop transformations (as outer loop i,j of initialization and updates usually need to keep in sync with each other).

After loop transformations, we can move the initialization of Y’s element separate from the reduction update. We can do that through the decompose_reduction primitive. (note: this is also done implicitly by tvm during future compilation, so this step is mainly to make it explicit and see the end effect).

sch.decompose_reduction(block_Y, k)
IPython.display.Code(sch.mod.script(), language="python")
Copy to clipboard
The final transformed code resembles the following low-level NumPy code.

def lnumpy_mm_relu_v3(A: np.ndarray, B: np.ndarray, C: np.ndarray):
    Y = np.empty((128, 128), dtype="float32")
    for i in range(128):
        for j0 in range(32):
            # Y_init
            for j1 in range(4):
                j = j0 * 4 + j1
                Y[i, j] = 0
            # Y_update
            for k in range(128):
                for j1 in range(4):
                    j = j0 * 4 + j1
                    Y[i, j] = Y[i, j] + A[i, k] * B[k, j]
            # C
            for j1 in range(4):
                j = j0 * 4 + j1
                C[i, j] = max(Y[i, j], 0)

c_np = np.empty((128, 128), dtype=dtype)
lnumpy_mm_relu_v3(a_np, b_np, c_np)
np.testing.assert_allclose(c_mm_relu, c_np, rtol=1e-5)
Copy to clipboard
2.4.4.2. Section Summary and Discussions

The main takeaway of this section is to get used to the paradigm of incremental code transformations. In our particular example, we use tir.Schedule as an auxiliary helper object.

Importantly, we avoided the need to re-create different variants of the same program (lnumpy_mm_relu, lnumpy_mm_relu_v2 and lnumpy_mm_relu_v3). The additional information in blocks (axes information) is the reason we can do such transformations under the hood.

2.4.5. Build and Run

So far, we have only looked at the script output of the transformed result. We can also run the program obtained in IRModule.

First, we call a build function to turn an IRModule into a runtime.Module, representing a collection of runnable functions. Here target specifies detailed information about the deployment environment. For this particular case, we will use llvm, which helps us compile to the native CPU platform.

When we target different platforms(e.g. an Android phone) or platforms with special instructions (intel skylake), we will need to adjust the target accordingly. We will discuss different target choices as we start to deploy to those environments.

rt_lib = tvm.build(MyModule, target="llvm")
Copy to clipboard
Then, we will create three tvm ndarrays that are used to hold inputs and the output.

a_nd = tvm.nd.array(a_np)
b_nd = tvm.nd.array(b_np)
c_nd = tvm.nd.empty((128, 128), dtype="float32")
type(c_nd)
Copy to clipboard
tvm.runtime.ndarray.NDArray
Finally, we can get the runnable function from rt_lib and execute it by passing the three array arguments. We can further run validation to check the code difference.

func_mm_relu = rt_lib["mm_relu"]
func_mm_relu(a_nd, b_nd, c_nd)

np.testing.assert_allclose(c_mm_relu, c_nd.numpy(), rtol=1e-5)
Copy to clipboard
We have built and run the original MyModule. We can also build the transformed program.

rt_lib_after = tvm.build(sch.mod, target="llvm")
rt_lib_after["mm_relu"](a_nd, b_nd, c_nd)
np.testing.assert_allclose(c_mm_relu, c_nd.numpy(), rtol=1e-5)
Copy to clipboard
Finally, we can compare the time difference between the two. time_evaluator is a helper benchmarking function that can be used to compare the running performance of different generated functions.

f_timer_before = rt_lib.time_evaluator("mm_relu", tvm.cpu())
print("Time cost of MyModule %g sec" % f_timer_before(a_nd, b_nd, c_nd).mean)
f_timer_after = rt_lib_after.time_evaluator("mm_relu", tvm.cpu())
print("Time cost of transformed sch.mod %g sec" % f_timer_after(a_nd, b_nd, c_nd).mean)
Copy to clipboard
Time cost of MyModule 0.00252932 sec
Time cost of transformed sch.mod 0.000657921 sec
It is interesting to see the running time difference between the two codes. Let us do a quick analysis of what are the possible factors that affect the performance. First, let us remind ourselves of two variants of the code.

import IPython

IPython.display.Code(MyModule.script(), language="python")
Copy to clipboard
# from tvm.script import ir as I
# from tvm.script import tir as T

@I.ir_module
class Module:
    @T.prim_func
    def mm_relu(A: T.Buffer((128, 128), "float32"), B: T.Buffer((128, 128), "float32"), C: T.Buffer((128, 128), "float32")):
        T.func_attr({"tir.noalias": T.bool(True)})
        # with T.block("root"):
        Y = T.alloc_buffer((128, 128))
        for i, j, k in T.grid(128, 128, 128):
            with T.block("Y"):
                vi, vj, vk = T.axis.remap("SSR", [i, j, k])
                T.reads(A[vi, vk], B[vk, vj])
                T.writes(Y[vi, vj])
                with T.init():
                    Y[vi, vj] = T.float32(0.0)
                Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]
        for i, j in T.grid(128, 128):
            with T.block("C"):
                vi, vj = T.axis.remap("SS", [i, j])
                T.reads(Y[vi, vj])
                T.writes(C[vi, vj])
                C[vi, vj] = T.max(Y[vi, vj], T.float32(0.0))
IPython.display.Code(sch.mod.script(), language="python")
Copy to clipboard
# from tvm.script import ir as I
# from tvm.script import tir as T

@I.ir_module
class Module:
    @T.prim_func
    def mm_relu(A: T.Buffer((128, 128), "float32"), B: T.Buffer((128, 128), "float32"), C: T.Buffer((128, 128), "float32")):
        T.func_attr({"tir.noalias": T.bool(True)})
        # with T.block("root"):
        Y = T.alloc_buffer((128, 128))
        for i, j_0 in T.grid(128, 32):
            for k, j_1 in T.grid(128, 4):
                with T.block("Y"):
                    vi = T.axis.spatial(128, i)
                    vj = T.axis.spatial(128, j_0 * 4 + j_1)
                    vk = T.axis.reduce(128, k)
                    T.reads(A[vi, vk], B[vk, vj])
                    T.writes(Y[vi, vj])
                    with T.init():
                        Y[vi, vj] = T.float32(0.0)
                    Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]
            for ax0 in range(4):
                with T.block("C"):
                    vi = T.axis.spatial(128, i)
                    vj = T.axis.spatial(128, j_0 * 4 + ax0)
                    T.reads(Y[vi, vj])
                    T.writes(C[vi, vj])
                    C[vi, vj] = T.max(Y[vi, vj], T.float32(0.0))
../_images/cpu_arch.png
To see why different loop variants result in different performances, we need to review the fact that it is not uniformly fast to access any piece of memory in A and B. Modern CPU comes with multiple levels of caches, where data needs to be fetched into the cache before the CPU can access it.

Importantly, it is much faster to access the data already in the cache. One strategy that CPU takes is to fetch data closer to each other. When we read one element in the memory, it will attempt to fetch the elements close by (formally known as cache-line) to the cache. So when you read the next element, it is already in the cache. As a result, code with continuous memory access is usually faster than code that randomly accesses different parts of the memory.

../_images/tensor_func_loop_order.png
Now let us look at the above visualization of iterations and analyze what is going on. In this analysis, let us focus on two inner-most loops: k and j1. The highlighted cover shows the corresponding region in Y, A and B that the iteration touches when we iterate over j1 for one specific instance of k.

We can find that the j1 iteration produces continuous access to elements of B. Specifically, it means the values we read when j1=0 and j1=1 are next to each other. This enables better cache access behavior. In addition, we bring the computation of C closer to Y, enabling better caching behavior.

Our current example is mainly to demonstrate that different variants of code can lead to different performances. More transformation steps can help us to get to even better performance, which we will cover in future chapters. The main goal of this exercise is first to get us the tool of program transformations and first taste of what is possible through transformations.

2.4.5.1. Exercise

As an exercise, try different j_factor choices and see how they affect the code’s performance.

def transform(mod, jfactor):
    sch = tvm.tir.Schedule(mod)
    block_Y = sch.get_block("Y", func_name="mm_relu")
    i, j, k = sch.get_loops(block_Y)
    j0, j1 = sch.split(j, factors=[None, jfactor])
    sch.reorder(j0, k, j1)
    block_C = sch.get_block("C", "mm_relu")
    sch.reverse_compute_at(block_C, j0)
    return sch.mod

mod_transformed = transform(MyModule, jfactor=8)

rt_lib_transformed = tvm.build(mod_transformed, "llvm")
f_timer_transformed = rt_lib_transformed.time_evaluator("mm_relu", tvm.cpu())
print("Time cost of transformed mod_transformed %g sec" % f_timer_transformed(a_nd, b_nd, c_nd).mean)
# display the code below
IPython.display.Code(mod_transformed.script(), language="python")
Copy to clipboard
Time cost of transformed mod_transformed 0.000380994 sec
# from tvm.script import ir as I
# from tvm.script import tir as T

@I.ir_module
class Module:
    @T.prim_func
    def mm_relu(A: T.Buffer((128, 128), "float32"), B: T.Buffer((128, 128), "float32"), C: T.Buffer((128, 128), "float32")):
        T.func_attr({"tir.noalias": T.bool(True)})
        # with T.block("root"):
        Y = T.alloc_buffer((128, 128))
        for i, j_0 in T.grid(128, 16):
            for k, j_1 in T.grid(128, 8):
                with T.block("Y"):
                    vi = T.axis.spatial(128, i)
                    vj = T.axis.spatial(128, j_0 * 8 + j_1)
                    vk = T.axis.reduce(128, k)
                    T.reads(A[vi, vk], B[vk, vj])
                    T.writes(Y[vi, vj])
                    with T.init():
                        Y[vi, vj] = T.float32(0.0)
                    Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]
            for ax0 in range(8):
                with T.block("C"):
                    vi = T.axis.spatial(128, i)
                    vj = T.axis.spatial(128, j_0 * 8 + ax0)
                    T.reads(Y[vi, vj])
                    T.writes(C[vi, vj])
                    C[vi, vj] = T.max(Y[vi, vj], T.float32(0.0))
2.4.6. Ways to Create and Interact with TensorIR

In the last sections, we learned about TensorIR abstraction and ways to transform things. TensorIR comes with an additional construct named block that helps us analyze and perform code transformations. One natural question we might ask: what are common ways to create and interact with TensorIR functions?

2.4.6.1. Create TensorIR via TVMScript

The first way to get a TensorIR function is to write a function in TVMScript directly, and this is also the approach we use in the last sections. TVMScript also allows us to skip certain parts of information when necessary. For example, T.axis.remap enables us to shorten the iterator size annotations.

TVMScript is also a useful way to inspect the tensor functions in the middle of transformations. In some instances, it might be helpful to print out the script, do some manual editing, then feed it back to the MLC process just to debug and try out possible transformation (manually), then bake it into the MLC process.

2.4.6.2. Generate TensorIR code using Tensor Expression

In many cases, our development forms are higher-level abstractions that are not at the loop level. So another common way to obtain TensorIR is programmatically generating relevant code.

Tensor expression (te) is a domain-specific language that describes a sequence of computations via an expression-like API.

from tvm import te

A = te.placeholder((128, 128), "float32", name="A")
B = te.placeholder((128, 128), "float32", name="B")
k = te.reduce_axis((0, 128), "k")
Y = te.compute((128, 128), lambda i, j: te.sum(A[i, k] * B[k, j], axis=k), name="Y")
C = te.compute((128, 128), lambda i, j: te.max(Y[i, j], 0), name="C")
Copy to clipboard
Here te.compute takes the signature te.compute(output_shape, fcompute). And the fcompute function describes how we want to compute the value of each element Y[i, j] for a given index.

lambda i, j: te.sum(A[i, k] * B[k, j], axis=k)
Copy to clipboard
The above lambda expression describes the computation . After describing the computation, we can create a TensorIR function by passing the relevant parameter we are interested in. In this particular case, we want to create a function with two input parameters (A, B) and one output parameter (C).

te_func = te.create_prim_func([A, B, C]).with_attr({"global_symbol": "mm_relu"})
MyModuleFromTE = tvm.IRModule({"mm_relu": te_func})
IPython.display.Code(MyModuleFromTE.script(), language="python")
Copy to clipboard
# from tvm.script import ir as I
# from tvm.script import tir as T

@I.ir_module
class Module:
    @T.prim_func
    def mm_relu(A: T.Buffer((128, 128), "float32"), B: T.Buffer((128, 128), "float32"), C: T.Buffer((128, 128), "float32")):
        T.func_attr({"tir.noalias": T.bool(True)})
        # with T.block("root"):
        Y = T.alloc_buffer((128, 128))
        for i, j, k in T.grid(128, 128, 128):
            with T.block("Y"):
                v_i, v_j, v_k = T.axis.remap("SSR", [i, j, k])
                T.reads(A[v_i, v_k], B[v_k, v_j])
                T.writes(Y[v_i, v_j])
                with T.init():
                    Y[v_i, v_j] = T.float32(0.0)
                Y[v_i, v_j] = Y[v_i, v_j] + A[v_i, v_k] * B[v_k, v_j]
        for i, j in T.grid(128, 128):
            with T.block("C"):
                v_i, v_j = T.axis.remap("SS", [i, j])
                T.reads(Y[v_i, v_j])
                T.writes(C[v_i, v_j])
                C[v_i, v_j] = T.max(Y[v_i, v_j], T.float32(0.0))
The tensor expression API provides a helpful tool to generate TensorIR functions for a given higher-level input.

2.4.7. TensorIR Functions as Results of Transformations

In practice, we also get TensorIR functions as results of transformations. This happens when we start with two primitive tensor functions (mm and relu), then apply a programmatic transformation to “fuse” them into a single primitive tensor function, mm_relu. We will cover the details in future chapters.

2.4.8. Discussions

In this section, let us review what we learned so far. We learned that a common MLC process follows a sequence of program transformations. It is interesting to compare the TensorIR transformation process to the low-level numpy reference development process.

../_images/standard_process.png
The above figure shows the standard development process. We need to repeat the process of developing different program variants and then (build if it is a compiled language) run them on the platform of interest.

The key difference in an MLC process(shown in the figure below) is the programmatic transformations among the IRModule (programs). So we can not only come up with program variants through development (either by manually writing the code or generating the code), but also can obtain variants by transforming the tensor programs.

Transformation is a very powerful tool that helps us simplify development costs and introduce more automation to the process. This section covered a specific perspective on primitive tensor functions via TensorIR, and we will cover more perspectives in the future.


def transform(mod, jfactor):
    sch = tvm.tir.Schedule(mod)
    block_Y = sch.get_block("Y", func_name="mm_relu")
    i, j, k = sch.get_loops(block_Y)
    j0, j1 = sch.split(j, factors=[None, jfactor])
    sch.reorder(j0, k, j1)
    block_C = sch.get_block("C", "mm_relu")
    sch.reverse_compute_at(block_C, j0)
    return sch.mod

mod_transformed = transform(MyModule, jfactor=8)

rt_lib_transformed = tvm.build(mod_transformed, "llvm")
f_timer_transformed = rt_lib_transformed.time_evaluator("mm_relu", tvm.cpu())
print("Time cost of transformed mod_transformed %g sec" % f_timer_transformed(a_nd, b_nd, c_nd).mean)
# display the code below
IPython.display.Code(mod_transformed.script(), language="python")

sec
# from tvm.script import ir as I
# from tvm.script import tir as T

@I.ir_module
class Module:
    @T.prim_func
    def mm_relu(A: T.Buffer((128, 128), "float32"), B: T.Buffer((128, 128), "float32"), C: T.Buffer((128, 128), "float32")):
        T.func_attr({"tir.noalias": T.bool(True)})
        # with T.block("root"):
        Y = T.alloc_buffer((128, 128))
        for i, j_0 in T.grid(128, 16):
            for k, j_1 in T.grid(128, 8):
                with T.block("Y"):
                    vi = T.axis.spatial(128, i)
                    vj = T.axis.spatial(128, j_0 * 8 + j_1)
                    vk = T.axis.reduce(128, k)
                    T.reads(A[vi, vk], B[vk, vj])
                    T.writes(Y[vi, vj])
                    with T.init():
                        Y[vi, vj] = T.float32(0.0)
                    Y[vi, vj] = Y[vi, vj] + A[vi, vk] * B[vk, vj]
            for ax0 in range(8):
                with T.block("C"):
                    vi = T.axis.spatial(128, i)
                    vj = T.axis.spatial(128, j_0 * 8 + ax0)
                    T.reads(Y[vi, vj])
                    T.writes(C[vi, vj])
                    C[vi, vj] = T.max(Y[vi, vj], T.float32(0.0))

from tvm import te

A = te.placeholder((128, 128), "float32", name="A")
B = te.placeholder((128, 128), "float32", name="B")
k = te.reduce_axis((0, 128), "k")
Y = te.compute((128, 128), lambda i, j: te.sum(A[i, k] * B[k, j], axis=k), name="Y")
C = te.compute((128, 128), lambda i, j: te.max(Y[i, j], 0), name="C")

 /cfml
  Application.cfc        ← handles request initialization, routing
  /api
    routes.cfm / endpoints
    detection_handler.cfm
    anomaly_handler.cfm
  /components
    SensorGateway.cfc     ← interface for sensors (camera, IoT)
    DetectorBridge.cfc    ← calls external inference module(s)
    Orchestrator.cfc      ← the “aura” brain
    Teleportation.cfc     ← sync module across devices
    QuantumSimulator.cfc  ← simulation / branching
    EventHistory.cfc      ← time-travel / replay logic
  /utils
    HttpClient.cfc
    JSONUtils.cfc
    Config.cfc
    Logger.cfc
