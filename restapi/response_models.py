# -*- coding: utf-8 -*-

# region Description
"""
request_models.py: Model HTTP response
Author: johndeweyzxc (johndewey02003@gmail.com)
"""
# endregion

from pydantic import BaseModel


class HashGetResponse(BaseModel):
    id: int
    ssid: str
    bssid: str
    client_mac_address: str
    key_type: str
    a_nonce: str
    hash_data: str
    key_data: str
    latitude: str
    longitude: str
    address: str
    date_captured: str


class HashUploadResponse(BaseModel):
    hash_data: str


class HashGetAllResponse(BaseModel):
    hash_list: list[HashGetResponse]


class HashGetRootResponse(BaseModel):
    message: str
