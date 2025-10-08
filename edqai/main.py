from fastapi import FastAPI
from edqai.api.optimize_routes import router as optimize_router

app = FastAPI(title="EDQ-AI Optimization API")

app.include_router(optimize_router)

@app.get("/")
def root():
    return {"message": "Welcome to EDQ-AI Optimizations"}
