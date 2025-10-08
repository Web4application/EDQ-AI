from fastapi import APIRouter
from pydantic import BaseModel
import numpy as np
from edqai.optimizations.tvm_mm_relu import optimize_mm_relu

router = APIRouter(prefix="/optimize", tags=["Optimization"])

class MatrixPair(BaseModel):
    A: list[list[float]]
    B: list[list[float]]

@router.post("/matrix")
def optimize_matrix(data: MatrixPair):
    A = np.array(data.A, dtype=np.float32)
    B = np.array(data.B, dtype=np.float32)
    result = optimize_mm_relu(A, B)
    return {"optimized_matrix": result.tolist()}
