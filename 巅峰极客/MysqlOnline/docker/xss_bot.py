# coding: utf-8
import requests
import selenium
from selenium import webdriver
import time

#session_path_file = '/tmp/xss_bot_session'
#base_url = 'http://127.0.0.1:8081/ctf/ichunqiu2018/mysqlonline/'
base_url = 'http://127.0.0.1/'
sleep_time_val = 1

chrome_options = webdriver.ChromeOptions()
chrome_options.add_argument('--headless')
chrome_options.add_argument('--disable-gpu')
chrome_options.add_argument('--no-sandbox')
chrome_options.add_argument("user-agent=Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36")

#client = webdriver.Chrome(chrome_options=chrome_options, executable_path='/home/l3m0n/Desktop/tools/chrome/chromedriver')
client = webdriver.Chrome(chrome_options=chrome_options, executable_path='/root/chromedriver')


# admin login
client.get(base_url + '/admin_zzzz666.php?pass=l3m0nl3m0nl3m0nBot*123')
client.get(base_url + '/admin_zzzz666.php')
print 'Xss Bot Satrt...'

while True:
    try:
        #print 1
        # get requests url from api
        try:
            r = requests.get(base_url + '/xss_bot_api_abcdefg_2576438.php?pass=youdontkonw', timeout=3)
            url = r.content
        except Exception,e:
            time.sleep(sleep_time_val)
            continue

        if url == 'Nothing':
            time.sleep(sleep_time_val)
            continue

        js = 'window.open("'+ url +'");'
        #client.execute_script("window.confirm = function(){return true;}");
        client.execute_script(js)
        #exit()
        handlesList = client.window_handles
        client.switch_to.window(handlesList[1])
        
        # content = client.page_source.encode('utf-8')
        # print content

        # only open a new web page
        handlesList = client.window_handles
        print handlesList
        if len(handlesList) > 2:
            for i in range(1, len(handlesList)):
                client.switch_to.window(handlesList[i])
                client.close()

        client.switch_to.window(handlesList[0])

        time.sleep(sleep_time_val)

    except selenium.common.exceptions.WebDriverException,e:
        #print e
        pass

client.quit()