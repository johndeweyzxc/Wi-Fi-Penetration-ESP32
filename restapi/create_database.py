# -*- coding: utf-8 -*-

# region Description
"""
create_database.py: Creates a database and stores it in the file hash_db.db
Author: johndeweyzxc (johndewey02003@gmail.com)
"""
# endregion

from sqlalchemy import create_engine, Column, Integer, String, Boolean
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker

Base = declarative_base()


class HashEntity(Base):
    __tablename__ = "hash_info"
    id = Column("id", Integer, primary_key=True)

    ssid = Column("ssid", String)
    bssid = Column("bssid", String)
    client_mac_address = Column("client_mac_address", String)
    key_type = Column("key_type", String)
    a_nonce = Column("a_nonce", String)
    hash_data = Column("hash_data", String)
    key_data = Column("key_data", String)
    date_captured = Column("date_captured", String)

    latitude = Column("latitude", String)
    longitude = Column("longitude", String)
    address = Column("address", String)

    cracked = Column("is_cracked", Boolean)
    psk = Column("psk", String)
    has_hc22000 = Column("has_hc22000", Boolean)

    def __init__(self, id, ssid, bssid, client_mac_address, key_type, a_nonce,
                 hash_data, key_data, latitude, longitude, address, date_captured,
                 cracked=False, psk="None", has_hc22000=False):
        self.id = id

        self.ssid = ssid
        self.bssid = bssid
        self.client_mac_address = client_mac_address
        self.key_type = key_type
        self.a_nonce = a_nonce
        self.hash_data = hash_data
        self.key_data = key_data

        self.latitude = latitude
        self.longitude = longitude
        self.address = address

        self.date_captured = date_captured

        self.cracked = cracked
        self.psk = psk
        self.has_hc22000 = has_hc22000


def main():
    engine = create_engine("sqlite:///hash_db.db", echo=True)
    Base.metadata.create_all(bind=engine)

    Session = sessionmaker(bind=engine)
    session = Session()

    # When the database is created this will be the first item
    id = 0

    ssid = "test"
    bssid = "test"
    client_mac_address = "test"
    key_type = "test"
    a_nonce = "test"
    hash_data = "test"
    key_data = "test"
    latitude = "0.0"
    longitude = "0.0"
    address = "test"
    date_captured = "test"

    hash_entity = HashEntity(
        id, ssid, bssid, client_mac_address, key_type,
        a_nonce, hash_data, key_data, latitude, longitude,
        address, date_captured)

    session.add(hash_entity)
    session.commit()


if __name__ == "__main__":
    main()
