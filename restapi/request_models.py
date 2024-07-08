# -*- coding: utf-8 -*-

# region Description
"""
request_models.py: Model HTTP post
Author: johndeweyzxc (johndewey02003@gmail.com)
"""
# endregion

from pydantic import BaseModel


class HashUploadRequest(BaseModel):
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
