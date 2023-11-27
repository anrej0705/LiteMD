README.MD

Этот файл был создан в LiteMD alpha 0.1.5 build 782
Исходный код на [GitHub](https://github.com/anrej0705/LiteMD/tree/LiteMD_alpha782)
Релизы: <https://github.com/anrej0705/LiteMD/releases>

Что сейчас может прога:
- Открывать, сохранять и редактировать
- Парсить ссылки URL например <https://www.google.com>
- Парсить альтерантивные ссылки URL, например [google](www.google.com)

Существующие на данный момент баги:
- Краш при написании альтеративных ссылок, если сначала написать открывающуюся круглую скобку после квадратных
- При закрытии программы с пустым документом появляется диалоговое окно подтверждения сохранения
- При открытии и закрыти программы без изменений в текстовом поле появляется окно подтверждения сохранения
- Упрощённый URL не работает в конструкции <>, например <www.google.com>
- Окно просмотра изображений не закрывается при закрытии программы
- Визуальный баг наложения ссылок при написании конструкции с упрощённым в URL внутри конструкции с альтернативным URL
- Не работают горячие клавиши
- При редактировании файла после первого сохранения флаг "*" не появляется при последующих редактированиях
- После сохранения отредактированного файла при закрытии всё равно будет вызван диалог подтверждения сохранения

Roadmap:

- :white_check_mark:0.0.1 - **Выход в альфу**
- :white_check_mark:0.1.0 - **URL ссылки**
- :white_check_mark:0.1.1 - Строка состояния
- :white_check_mark:0.1.2 - Иконка приложения
- :white_check_mark:0.1.4 - Модуль загрузки по HTTP
- :white_check_mark:0.1.5 - Прототип окна настроек
- :x:0.1.7 - Базовые настройки
- :x:0.1.8 - Иконки на кнопках меню и дока
- :x:0.1.9 - Списки изменений от версии к версии
- :x:0.2.0 - **Картинки и настройки**
- :x:0.3.0 - **Списки и цитаты**
- :x:0.4.0 - **Таблицы и разделители**
- :x:0.5.0 - **Дополнительное форматирование текста**
- :x:0.6.0 - **Параграфы и строки**
- :x:0.7.0 - **Вставка кода и экспорт в PDF**
- :x:0.8.0 - **Бета**
- :x:0.8.1 - **Внешний дизайн**
- :x:0.8.2 - **Проверка обновлений**
- :x:0.8.3 - **MDI приложение**
- :x:0.8.4 - **Эмодзи**
- :x:0.9.0 - **Воспроизведение видео по ссылкам**
- :x:0.9.1 - **Система плагинов**
- :x:1.0.0 - **Релиз и дальнейшая разработка**
