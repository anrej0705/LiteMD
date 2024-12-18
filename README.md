README.MD

# LiteMD alpha 0.2.8

![Qt](https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![MSVC](https://img.shields.io/badge/Visual_Studio-5C2D91?style=for-the-badge&logo=visual%20studio&logoColor=white)
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)

**| Qt 5.14.2 
\| C++17 
\| C11 
\| MSVC 2019 
\| GPL v2 license 
\| boost 1.84.00 |**

Этот файл был создан в LiteMD alpha 0.2.3 build 1829
Исходный код на [GitHub](https://github.com/anrej0705/LiteMD)

Релизы: <https://github.com/anrej0705/LiteMD/releases>

[CONTRIBUTING/Рекомендации по созданию пул реквестов](https://github.com/anrej0705/LiteMD/blob/master/CONTRIBUTING.md)

Текущие возможности программы:
- Редактирование Markdown(\*.md) файлов
- Редактирование Text-Markdown(\*.txt) файлов
- Парсинг простых ссылок по типу <https://www.google.com>
- Парсинг альтернативных ссылок, как пример [google](www.google.com)
- Парсинг заголовков H1-H5 `#`-`#####`
- Гибкая настройка обработки текста
- Загрузка картинок по протоколу HTTP
- Динамическая смена языка
- Просмотр списка изменений в текущей версии
- Обработка ~~зачёркнутого~~ текста
- Обработка <ins>подчёркнутого</ins> текста
- Обработка *курсива*
- Тулбар инструментов форматирования и работы с текстом
- Обработка **жирного**

Существующие на данный момент баги:
- :x:Ошибки и вылеты при работе с устаревшими парсерами(просто не используйте их)
- :x:При определенных действиях хоткей сохранения файла может быть воспринят как "Сохранить как"

#### **Roadmap**:

- :white_check_mark:rc - [релиз-кандидат](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_rc)
- :white_check_mark:0.0.1 - [**Выход в альфу**](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha84)
- :white_check_mark:0.0.2 - Панель меню сверху(не выложен)
- :white_check_mark:0.0.3 - Кнопки прокрутки(не выложен)
- :white_check_mark:0.0.4 - Скроллинг текста в окнах рендера и редактора(не выложен)
- :white_check_mark:0.0.5 - Сохранение файлов в .MD(не выложен)
- :white_check_mark:0.0.6 - Окно "О программе"(не выложен)
- :white_check_mark:0.0.7 - Динамическое обновление текста в рендере(не выложен)
- :white_check_mark:0.0.8 - Смена заголовка `lmd prj` -> `LiteMD`(не выложен)
- :white_check_mark:0.0.9 - Исправления - интерфейс(не выложен)
- :white_check_mark:0.1.0 - **URL ссылки**(не выложен)
- :white_check_mark:0.1.1 - [Строка состояния](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha312)
- :white_check_mark:0.1.2 - [Иконка приложения](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha356)
- :white_check_mark:0.1.3 - [Исправление - привязка к разрешению](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha428)
- :white_check_mark:0.1.4 - [Модуль загрузки по HTTP](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha455)
- :white_check_mark:0.1.5 - [Прототип окна настроек](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha782)
- :white_check_mark:0.1.6 - [Исправления](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha842)
- :white_check_mark:0.1.7 - [Базовые настройки](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha1006)
- :white_check_mark:0.1.8 - [Иконки на кнопках меню и дока](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha1064)
- :white_check_mark:0.1.9 - [Списки изменений от версии к версии](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha1225)
- :white_check_mark:0.2.0 - [**Дополнительное форматирование текста**](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha1554)
- :white_check_mark:0.2.1 - [Уровни заголовков](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha1683)
- :white_check_mark:0.2.2 - [Окно логов](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha1746)
- :white_check_mark:0.2.3 - [Импорт старых конфигов](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha1829)
- :white_check_mark:0.2.4 - [Зачеркнутый текст](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha1878)
- :white_check_mark:0.2.5 - [Подчеркнутый текст](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha1908) 
- :white_check_mark:0.2.6 - [Курсив](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha1917)
- :white_check_mark:0.2.7 - [Жирный текст](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha1941)
- :white_check_mark:0.2.8 - [Комбинирование форматирования текста](https://github.com/anrej0705/LiteMD/releases/tag/LiteMD_alpha1957)
- :arrow_forward:0.2.9 - Меню недавних файлов от [SilverWolf2020](https://github.com/anrej0705/LiteMD/pull/2)
- :x:0.3.0 - **Списки и цитаты**
- :x:0.3.1 - Списки
- :x:0.3.2 - Цитирование текста
- :x:0.3.3 - Последний редактируемый фаил
- :x:0.3.4 - Раздел справки
- :x:0.3.5 - Режим читалки
- :x:0.3.6 - Всплывающая подсказка и подсказка "Что это?"
- :x:0.3.7 - Проверка обновлений через GitHub API
- :x:0.3.8 - Новый загрузчик
- :x:0.3.9 - Сохранение настроек
- :x:0.4.0 - **Таблицы и разделители**
- :x:0.4.1 - Разделители строк
- :x:0.4.2 - Настройка очереди парсеров
- :x:0.4.3 - Обработка некоторых исключений и доп параметры меню
- :x:0.4.4 - Дополнительное логирование
- :x:0.4.5 - Базовые таблицы
- :x:0.4.6 - Альтернативные таблицы
- :x:0.4.7 - Форматированный текст в таблицах
- :x:0.4.8 - Выравнивание текста
- :x:0.4.9 - Экранирование служебных символов
- :x:0.5.0 - **Параграфы и строки**
- :x:0.5.1 - Вставка параграфов
- :x:0.5.2 - Ссылки-сноски
- :x:0.5.3 - Подсветка синтаксиса в редакторе
- :x:0.5.4 - Дополнительные языки локализации
- :x:0.5.5 - Запрос настроек по сети
- :x:0.5.6 - Режим разработчика(для разработчиков)
- :x:0.5.7 - Проверка орфографии
- :x:0.5.8 - Улучшенная система исключений(мы заботимся о ваших файлах)
- :x:0.5.9 - Генерация отчётов об ошибках
- :x:0.6.0 - **Картинки и настройки**
- :x:0.6.1 - Дополнительные разделы настроек
- :x:0.6.2 - Автосейв
- :x:0.6.3 - Настройки автосейва
- :x:0.6.4 - Настройки рендера(реализация механики)
- :x:0.6.5 - Кастомизация парсеров
- :x:0.6.6 - Сетевые настройки
- :x:0.6.7 - Вставка картинок
- :x:0.6.8 - Кеширование картинок
- :x:0.6.9 - Графическая справка(с картинками-примерами)
- :x:0.7.0 - **Вставка кода и экспорт в PDF**
- :x:0.7.1 - Вставка кода
- :x:0.7.2 - Подсветка синтаксиса кода
- :x:0.7.3 - Юзер-бинды
- :x:0.7.4 - Разделение настроек на "Простые" и "Расширенные"
- :x:0.7.5 - Экспорт файла в PDF
- :x:0.7.6 - Потоковая конвертация в PDF
- :x:0.7.9 - Новые иконки
- :x:0.8.0 - **Бета**
- :x:0.8.1 - Внешний дизайн
- :x:0.8.2 - Проверка обновлений(запрос GitHub)
- :x:0.8.3 - MDI приложение
- :x:0.8.4 - **Эмодзи**
- :x:0.8.5 - GitHub эмодзи
- :x:0.8.6 - Скины(тёмный, гибрид)
- :x:0.8.7 - Пользовательские скины
- :x:0.9.0 - **Воспроизведение видео по ссылкам и плагины**
- :x:0.9.1 - Система плагинов
- :x:0.9.2 - Вставка видео(кликабельные превью)
- :x:0.9.3 - Пользовательские плагины
- :x:1.0.0 - **Релиз и дальнейшая разработка**

\*Содержимое Roadmap и невыполненные таски могут меняться/переставляться со временем

\*(Только для альфы и беты)Каждая версия начинающаяся с 0.x.0 не будет заключать в себя обещанные фичи из всей ветки 0.x.x а только списки исправлений и доработок. Фичи будут добавляться в соотвествии с поставленными тасками

\*Версии 0.0.2-0.0.9 отсутствуют в списке релизов
