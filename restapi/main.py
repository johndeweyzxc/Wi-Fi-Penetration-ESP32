# -*- coding: utf-8 -*-

# region Description
"""
main.py: The main file for launching the rest api
Author: johndeweyzxc (johndewey02003@gmail.com)
"""
# endregion

from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from fastapi import FastAPI, status
from fastapi.responses import JSONResponse
import uvicorn

from create_database import HashEntity
from request_models import HashUploadRequest
from response_models import HashGetResponse, HashUploadResponse, HashGetAllResponse, HashGetRootResponse

app = FastAPI()
Base = declarative_base()

engine = create_engine("sqlite:///hash_db.db", echo=True)
Base.metadata.create_all(bind=engine)

Session = sessionmaker(bind=engine)
session = Session()


@app.get("/", response_model=HashGetRootResponse)
async def read_root():
    return HashGetRootResponse(
        message="AWPS bridge for the command launch module")


@app.post("/api/v1/upload-hash", response_model=HashUploadResponse)
async def upload_hash(hash_upload: HashUploadRequest):
    hash_upload_dict: dict[str, any] = hash_upload.__dict__
    number_of_entities = session.query(HashEntity).count()
    print(hash_upload_dict)

    id = number_of_entities

    ssid = hash_upload_dict.get("ssid")
    bssid = hash_upload_dict.get("bssid")
    client_mac_address = hash_upload_dict.get("client_mac_address")
    key_type = hash_upload_dict.get("key_type")
    a_nonce = hash_upload_dict.get("a_nonce")
    hash_data = hash_upload_dict.get("hash_data")
    key_data = hash_upload_dict.get("key_data")

    latitude = hash_upload_dict.get("latitude")
    longitude = hash_upload_dict.get("longitude")
    address = hash_upload_dict.get("address")

    date_captured = hash_upload_dict.get("date_captured")

    hash_entity = HashEntity(
        id, ssid, bssid, client_mac_address, key_type,
        a_nonce, hash_data, key_data, latitude, longitude,
        address, date_captured)

    session.add(hash_entity)
    session.commit()
    return HashUploadResponse(hash_data=hash_entity.hash_data)


@app.get("/api/v1/get-all-hash", response_model=HashGetAllResponse)
async def get_all_hash():
    results = session.query(HashEntity).all()
    list_response = []

    for hash in results:
        hash_get_response = HashGetResponse(
            id=hash.id,
            ssid=hash.ssid,
            bssid=hash.bssid,
            client_mac_address=hash.client_mac_address,
            key_type=hash.key_type,
            a_nonce=hash.a_nonce,
            hash_data=hash.hash_data,
            key_data=hash.key_data,
            latitude=hash.latitude,
            longitude=hash.longitude,
            address=hash.address,
            date_captured=hash.date_captured)

        list_response.append(hash_get_response)

    return HashGetAllResponse(hash_list=list_response)


@app.get("/api/v1/get-hash/{id}", response_model=HashGetResponse)
async def get_hash(id: int):
    results = session.query(HashEntity).filter(HashEntity.id == id)
    hash = results[0]

    return HashGetResponse(
        id=hash.id,
        ssid=hash.ssid,
        bssid=hash.bssid,
        client_mac_address=hash.client_mac_address,
        key_type=hash.key_type,
        a_nonce=hash.a_nonce,
        hash_data=hash.hash_data,
        key_data=hash.key_data,
        latitude=hash.latitude,
        longitude=hash.longitude,
        address=hash.address,
        date_captured=hash.date_captured)


@app.get("/api/v1/delete-hash/{id}")
async def delete_hash(id: int):
    session.query(HashEntity).filter(HashEntity.id == id).delete()
    return JSONResponse(content=None, status_code=status.HTTP_204_NO_CONTENT)


if __name__ == "__main__":
    """ Change host depending on the assigned IPv4 on your device """
    uvicorn.run(app, host="192.168.1.9", port=8000)
