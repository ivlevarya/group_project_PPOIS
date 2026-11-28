from pathlib import Path
from typing import Literal

from argon2 import PasswordHasher
from fastapi import FastAPI, HTTPException, Depends 
from fastapi.security import HTTPBearer, HTTPAuthorizationCredentials
from pydantic import BaseModel, EmailStr, Field
import os
import jwt

from datetime import datetime, timedelta, timezone
from dotenv import load_dotenv


from api import medspase_core

app = FastAPI(title="MedSpase", version="0.1.0")

bearer_scheme = HTTPBearer(auto_error=False)

DB_PATH = str(
    Path(__file__).resolve().parents[1]
    / "project"
    / "database"
    / "medspace.db"
)


password_hasher = PasswordHasher()

load_dotenv(Path(__file__).resolve().parents[1] / ".env")

JWT_SECRET = os.getenv("MEDSPASE_JWT_SECRET")

if not JWT_SECRET or len(JWT_SECRET) < 32:
    raise RuntimeError("JWT secret is not configured")


class RegisterRequest(BaseModel):
    first_name: str
    last_name: str
    email: EmailStr
    password: str = Field(min_length=8)
    confirm_password: str
    accept_terms: Literal[True]

class LoginRequest(BaseModel):
    email: EmailStr
    password: str


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



from argon2.exceptions import VerifyMismatchError


def authenticate_user(email: str, password: str):
    user = medspase_core.get_user_by_email(DB_PATH, email)

    if user is None:
        return None

    try:
        password_hasher.verify(user["password_hash"],password)
    except VerifyMismatchError:
        return None

    return user


@app.post("/auth/login")
def login(request: LoginRequest):

    user = authenticate_user(str(request.email).lower(), request.password)

    if user is None:
        raise HTTPException(
            status_code=401,
            detail="Invalid email or password"
        )

    expires_at = (
        datetime.now(timezone.utc)
        + timedelta(minutes=45)
    )

    token = jwt.encode(
        {
            "sub": str(user["id"]),
            "exp": expires_at
        },
        JWT_SECRET,
        algorithm="HS256"
    )

    return {
        "access_token": token,
        "token_type": "bearer"
    }




def get_authenticated_user_id(
    credentials: HTTPAuthorizationCredentials | None = Depends(bearer_scheme)) -> int:

    if credentials is None:
        raise HTTPException(
            status_code=401,
            detail="Authentication required",
            headers={"WWW-Authenticate": "Bearer"}
        )

    try:
        payload = jwt.decode(
            credentials.credentials,
            JWT_SECRET,
            algorithms=["HS256"],
            options={"require": ["sub", "exp"]}
        )

        user_id = int(payload["sub"])

        if user_id <= 0:
            raise ValueError("Invalid user ID")

    except (jwt.InvalidTokenError, ValueError, TypeError):
        raise HTTPException(
            status_code=401,
            detail="Invalid or expired token",
            headers={"WWW-Authenticate": "Bearer"}
        )

    return user_id



@app.get("/auth/me")
def get_me(user_id: int = Depends(get_authenticated_user_id)):
    return {
        "id": user_id
    }


