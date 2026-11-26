from fastapi import FastAPI

app = FastAPI(
    title="MedSpase API",
    version="0.1.0"
)


@app.get("/")
def home():
    return {"message": "Welcome to MedSpase API"}


@app.get("/health")
def health():
    return {"status": "ok"}
