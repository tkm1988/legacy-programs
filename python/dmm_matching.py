from googlesearch import search
from bs4 import BeautifulSoup
import requests as web
import pandas as pd

dmm_title_list = []
 wrong_list = []

for avid in df["ID"] :
    for target_url in googlesearch.search(avid + " dmm", lang="ja", stop=1):
        if "dmm" in target_url :
            response = web.get(target_url)
            soup = BeautifulSoup(response.text, "lxml")
            target_items = soup.find_all("h1")
            for item in target_items :
                dmm_title_list.append([str(avid), item.string, str(target_url)])
        else :
            wrong_list.append([str(avid),str(target_url)])
