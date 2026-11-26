from pathlib import Path
from typing import Literal

from argon2 import PasswordHasher
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel, EmailStr, Field


from api import medspase_core

app = FastAPI(title="MedSpase", version="0.1.0")

DB_PATH = str(
    Path(__file__).resolve().parents[1]
    / "project"
    / "database"
    / "medspace.db"
)


password_hasher = PasswordHasher()


class RegisterRequest(BaseModel):
    first_name: str
    last_name: str
    email: EmailStr
    password: str = Field(min_length=8)
    confirm_password: str
    accept_terms: Literal[True]


@app.post("/auth/register", status_code=201)
def register(request: RegisterRequest):

    if request.password != request.confirm_password:
        raise HTTPException(status_code=400, detail="Passwords do not match")

    first_name = request.first_name.strip()
    last_name = request.last_name.strip()
    email = str(request.email).lower()

    if not first_name or not last_name:
        raise HTTPException(status_code=400, detail="First name and last name are required")

    existing_user = medspase_core.get_user_by_email(DB_PATH, email)

    if existing_user is not None:
        raise HTTPException(status_code=409, detail="Email already registered")

    password_hash = password_hasher.hash(request.password)

    try:
        user_id = medspase_core.create_user(DB_PATH, first_name, last_name, email, password_hash)
    except Exception as exc:
        if "UNIQUE constraint failed" in str(exc):
            raise HTTPException(
                status_code=409,
                detail="Email already registered"
            ) from exc
        raise HTTPException(
            status_code=500,
            detail="Registration failed"
        ) from exc

    return {
        "id": user_id,
        "first_name": first_name,
        "last_name": last_name,
        "email": email
    }

