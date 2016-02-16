# -*- coding: UTF-8 -*-
#Кораблёв Анатолий, ПС-22

import os #проверка на существование папки, создание папки
import urllib #берем содержимое страницы
import re #поиск информации в тексте
import requests #для работы с http, парсинга информации
import urlparse #для вытаскивания хоста из url сайта

def correct_name(name, symbols):
    for i in symbols:
        name = name.replace(i, '')
    return name

def save_object(urls, path):
    print 'save_object'
    lst_name = []
    count = 0
    while count < len(urls): #проходим по ссылкам
        url = urls[count] #выгружает из списка ссылок одну ссылку
        name = correct_name(url, '/:*?"<>|') # удаление из имени недопустимых символов
        if ('https://' in url) or ('http://' in url) or ('ftp://' in url)\
           or ('ftps://' in url): # проверка на абсолютный путь
            check_name = name.split('.') 
            if check_name[-1].isalnum(): #проверка на корректное расширение (либо цифры, либо буквы и цифры)
                objec = requests.get(url) #переходим по ссылке, вытаскиваем содержимое
                lst_name.append(name) #добавляем в конец списка корректное имя
                fil = open(path + name, 'wb') #открываем файл для записи
                fil.write(objec.content) #записываем содержимое ссылки в файл
                fil.close()
        elif len(url) > 2:
            while (url[0] in ['/', ':']) and (len(url) > 2):
                url = url[1:]
            url = 'http://' + url #Замена относительных ссылок в абсолютные, дописываем http://
            check_name = name.split('.')
            if check_name[-1].isalnum():
                objec = requests.get(url)
                lst_name.append(name)
                fil = open(path + name, 'wb')
                fil.write(objec.content)
                fil.close()
        count += 1
    return lst_name

def save_site(url, path, num_site):
    print 'save_site'
    html = urllib.urlopen(url).read() #преобразовываем сайт в текст
    img_urls = re.findall('img .*?src="(.*?)"', html) # поиск ссылок изображений
    js_urls = re.findall('script .*?src="(http:*.*?)"', html) # поиск ссылок на шрифты
    css_urls = re.findall('rel="stylesheet" .*?href="(.*?css)"', html) # поиск css файлов
    path = path + str(num_site) + '/' # путь до папки с сайтов
    if not os.path.exists(path): #создание папки
        os.mkdir(path)
    path_obj = path + 'img/' # папка с изображениями
    if not os.path.exists(path_obj): #создание папки
        os.mkdir(path_obj)
    img_name = save_object(img_urls, path_obj)
    path_obj = path + 'js/'
    if not os.path.exists(path_obj): #создание папки
        os.mkdir(path_obj)
    js_name = save_object(js_urls, path_obj) 
    path_obj = path + 'css/'
    if not os.path.exists(path_obj): #создание папки
        os.mkdir(path_obj)
    css_name = save_object(css_urls, path_obj)
    for i in range(len(img_name)): # Происходит замена ссылок на файлы в интернете на ссылки на них на диске, замена абсолютных в относительные на объект на диске
        html = html.replace(img_urls[i], './img/' + img_name[i])
    for i in range(len(js_name)):
        html = html.replace(js_urls[i], './js/' + js_name[i])
    for i in range(len(css_name)):
        html = html.replace(css_urls[i], './css/' + css_name[i])
    fil = open(path + 'index.html', 'wb')
    fil.write(html)
    fil.close()
    print 'save_site_complete'

def read_and_add_url(url, lst, word, path, num_site, h_name): #чтение сайта и добавление всех ссылок сайта в очередь 
    print url
    html = urllib.urlopen(url).read()#записываем содержимое страницы в переменную
    if word in html: #если слово есть на сайте, проходим
        save_site(url, path, num_site)        
        links = re.findall('a .*?href="(.*?)"', html) # Список всех ссылок со страницы
        for i in range(len(links)): # проход по ссылкам
            a = links[i] 
            if not(('https://' in a) or ('http://' in a) or ('ftp://' in a)\
           or ('ftps://' izn a)): # проверка на не существования http:/ так, как ссылка на другой сайт относительная, без домена
                if (a[-1] == '/') and (len(a) > 3): 
                    a = 'http://'  + h_name + a # добавление http:// и домена lenta.ru
                    print a
                    if a not in lst: # проверка url в очереди, если нет то записываем. Чтобы исключить повторения
                        lst.append(str(a)) #добавление url в конец очереди
    return lst

    
word = "Маркин"
url = 'http://lenta.ru/'
host_name = urlparse.urlparse(url).hostname #вытаскиваем имя хоста (lenta.ru)
path = './download_sites/' 
if not os.path.exists(path): #проверка пути
    os.mkdir(path) #создание папки
lst_url = [url] #первый элемент очереди
num_site = 1 #номер папки с сайтом
lst_url = read_and_add_url(url, lst_url, word, path, num_site, host_name) #очередь
count = 0
while count != len(lst_url): #проход по очереди
    print count, len(lst_url)
    num_site += 1
    print lst_url[count]
    lst_url = read_and_add_url(lst_url[count], lst_url, word, path, num_site, host_name)
    count += 1
print 'Complete'

