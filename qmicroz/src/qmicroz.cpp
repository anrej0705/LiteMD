/*
 * This file is part of QMicroz,
 * under the MIT License.
 * https://github.com/artemvlas/qmicroz
 *
 * Author: Artem Vlasenko
 * https://github.com/artemvlas
*/
#include "qmicroz.h"
#include "tools.h"
#include <QDir>
#include <QDebug>

const QString QMicroz::s_zip_ext = QStringLiteral(u".zip");

QMicroz::QMicroz() {}

QMicroz::QMicroz(const char* zip_path)
{
    setZipFile(QString(zip_path));
}

QMicroz::QMicroz(const QString &zip_path)
{
    setZipFile(zip_path);
}

QMicroz::QMicroz(const QByteArray &buffered_zip)
{
    setZipBuffer(buffered_zip);
}

QMicroz::~QMicroz()
{
    closeArchive();
}

bool QMicroz::setZipFile(const QString &zip_path)
{
    if (QFileInfo(zip_path).isFile()) {
        // try to open zip archive
        mz_zip_archive *_za = tools::za_new(zip_path, tools::ZaReader);

        if (_za) {
            closeArchive();

            m_archive = _za;
            m_zip_path = zip_path;
            updateZipContents();
            return true;
        }
    }

    qWarning() << "Wrong path:" << zip_path;
    return false;
}

bool QMicroz::setZipBuffer(const QByteArray &buffered_zip)
{
    // open zip archive
    mz_zip_archive *_za = new mz_zip_archive();

    if (mz_zip_reader_init_mem(_za, buffered_zip.constData(), buffered_zip.size(), 0)) {
        // close currently opened if any
        closeArchive();

        // set the new one
        m_archive = _za;
        m_zip_path.clear();
        updateZipContents();
        return true;
    }

    qWarning() << "Failed to open buffered zip";
    delete _za;
    return false;
}

void QMicroz::setOutputFolder(const QString &output_folder)
{
    m_output_folder = output_folder;
}

const QString& QMicroz::outputFolder()
{
    if (m_output_folder.isEmpty()) {
        if (m_zip_path.isEmpty()) {
            // buffered zip with no m_output_folder setted
            qWarning() << "No output folder setted";
        }
        else {
            // zip file's parent folder
            m_output_folder = QFileInfo(m_zip_path).absolutePath();
        }
    }

    return m_output_folder;
}

void QMicroz::closeArchive()
{
    if (m_archive) {
        mz_zip_archive *_za = static_cast<mz_zip_archive *>(m_archive);
        tools::za_close(_za);
        m_archive = nullptr;
    }
}

const ZipContentsList& QMicroz::updateZipContents()
{
    m_zip_contents.clear();

    if (!m_archive)
        return m_zip_contents;

    mz_zip_archive *_za = static_cast<mz_zip_archive *>(m_archive);

    // iterating...
    const mz_uint _num_items = mz_zip_reader_get_num_files(_za);

    for (uint it = 0; it < _num_items; ++it) {
        const QString _filename = tools::za_item_name(_za, it);
        if (_filename.isEmpty()) {
            break;
        }

        m_zip_contents[it] = _filename;
    }

    return m_zip_contents;
}

const ZipContentsList& QMicroz::contents()
{
    if (m_zip_contents.isEmpty())
        updateZipContents();

    return m_zip_contents;
}

bool QMicroz::extractAll()
{
    if (!m_archive) {
        qDebug() << "No zip archive setted";
        return false;
    }

    if (outputFolder().isEmpty())
        return false;

    // zip file setted
    if (!m_zip_path.isEmpty())
        return extract(m_zip_path, outputFolder());


    // mem zip (buffered) setted
    return tools::extract_all_to_disk(static_cast<mz_zip_archive*>(m_archive), outputFolder());
}

// !recreate_path >> place in the root of the output folder
bool QMicroz::extractIndex(int file_index, bool recreate_path)
{
    if (!m_archive) {
        qDebug() << "No zip archive setted";
        return false;
    }

    if (file_index == -1 || outputFolder().isEmpty())
        return false;

    const QString &output_folder = outputFolder();

    qDebug() << "Extract:" << file_index << "from:" << m_zip_path;
    qDebug() << "Output folder:" << output_folder;

    // create output folder if it doesn't exist
    if (!tools::createFolder(output_folder)) {
        return false;
    }

    mz_zip_archive *_za = static_cast<mz_zip_archive *>(m_archive);

    // extracting...
    // the name is also a relative path inside the archive
    const QString _filename = tools::za_item_name(_za, file_index);
    if (_filename.isEmpty()) {
        return false;
    }

    qDebug() << "Extracting:" << _filename;
    const QString _outpath = tools::joinPath(output_folder,
                                             recreate_path ? _filename : QFileInfo(_filename).fileName());

    // create new path on the disk
    const QString _parent_folder = QFileInfo(_outpath).absolutePath();
    if (!tools::createFolder(_parent_folder)) {
        return false;
    }

    // subfolder, no data to extract
    if (_filename.endsWith(tools::s_sep)) {
        qDebug() << "Subfolder extracted";
    }
    // extract file
    else if (!tools::extract_to_file(_za, file_index, _outpath)) {
        return false;
    }

    qDebug() << "Unzip complete.";
    return true;
}

bool QMicroz::extractFile(const QString &file_name, bool recreate_path)
{
    return extractIndex(findIndex(file_name), recreate_path);
}

BufFileList QMicroz::extract_to_ram() const
{
    BufFileList _res;

    if (!m_archive) {
        qWarning() << "No archive setted";
        return _res;
    }

    qDebug() << "Extracting to RAM:" << (m_zip_path.isEmpty() ? "buffered zip" : m_zip_path);

    mz_zip_archive *_za = static_cast<mz_zip_archive *>(m_archive);

    // extracting...
    const mz_uint _num_items = mz_zip_reader_get_num_files(_za);

    for (uint it = 0; it < _num_items; ++it) {
        const QString _filename = tools::za_item_name(_za, it);
        if (_filename.isEmpty()) {
            break;
        }

        // subfolder, no data to extract
        if (_filename.endsWith(tools::s_sep))
            continue;

        qDebug() << "Extracting:" << (it + 1) << '/' << _num_items << _filename;

        // extract file
        const QByteArray _data = tools::extract_to_buffer(_za, it);
        if (!_data.isNull())
            _res[_filename] = _data;
    }

    qDebug() << "Unzipped:" << _res.size() << "files";
    return _res;
}

BufFile QMicroz::extract_to_ram(int file_index) const
{
    BufFile _res;

    if (!m_archive) {
        qWarning() << "No archive setted";
        return _res;
    }

    if (file_index == -1)
        return _res;

    qDebug() << "Extracting to RAM:" << (m_zip_path.isEmpty() ? "buffered zip" : m_zip_path);

    mz_zip_archive *_za = static_cast<mz_zip_archive *>(m_archive);

    const QString _filename = tools::za_item_name(_za, file_index);
    if (_filename.isEmpty())
        return _res;

    // subfolder, no data to extract
    if (_filename.endsWith(tools::s_sep)) {
        qDebug() << "Subfolder, no data to extract:" << _filename;
        return _res;
    }

    qDebug() << "Extracting:" << _filename;

    // extract file
    const QByteArray _data = tools::extract_to_buffer(_za, file_index);

    if (!_data.isNull()) {
        _res.m_name = _filename;
        _res.m_data = _data;

        qDebug() << "Unzipped:" << _data.size() << "bytes";
    }

    return _res;
}

BufFile QMicroz::extract_to_ram_file(const QString &file_name)
{
    return extract_to_ram(findIndex(file_name));
}

// STATIC functions ---->>>
bool QMicroz::extract(const QString &zip_path)
{
    // extract to parent folder
    return extract(zip_path, QFileInfo(zip_path).absolutePath());
}

bool QMicroz::extract(const QString &zip_path, const QString &output_folder)
{
    qDebug() << "Extract:" << zip_path;
    qDebug() << "Output folder:" << output_folder;

    // create output folder if it doesn't exist
    if (!tools::createFolder(output_folder)) {
        return false;
    }

    // open zip archive
    mz_zip_archive *__za = tools::za_new(zip_path, tools::ZaReader);
    if (!__za) {
        return false;
    }

    // extracting...
    bool is_success = tools::extract_all_to_disk(__za, output_folder);

    // finish
    tools::za_close(__za);
    return is_success;
}

bool QMicroz::compress_(const QString &path)
{
    QFileInfo __fi(path);

    if (__fi.isFile()) {
        return compress_file(path);
    }
    else if (__fi.isDir()) {
        return compress_folder(path);
    }
    else {
        qDebug() << "QMicroz::compress | WRONG path:" << path;
        return false;
    }
}

bool QMicroz::compress_(const QStringList &paths)
{
    if (paths.isEmpty())
        return false;

    const QString _rootfolder = QFileInfo(paths.first()).absolutePath();
    const QString _zipname = QFileInfo(_rootfolder).fileName() + s_zip_ext;
    const QString _zippath = tools::joinPath(_rootfolder, _zipname);

    return compress_list(paths, _zippath);
}

bool QMicroz::compress_file(const QString &source_path)
{
    QFileInfo __fi(source_path);
    const QString _zip_name = __fi.completeBaseName() + s_zip_ext;
    const QString _out_path = tools::joinPath(__fi.absolutePath(), _zip_name);

    return compress_file(source_path, _out_path);
}

bool QMicroz::compress_file(const QString &source_path, const QString &zip_path)
{
    if (!QFileInfo::exists(source_path)) {
        qWarning() << "File not found:" << source_path;
        return false;
    }

    return compress_list({ source_path }, zip_path);
}

bool QMicroz::compress_folder(const QString &source_path)
{
    QFileInfo __fi(source_path);
    const QString _file_name = __fi.fileName() + s_zip_ext;
    const QString _parent_folder = __fi.absolutePath();
    const QString _out_path = tools::joinPath(_parent_folder, _file_name);

    return compress_folder(source_path, _out_path);
}

bool QMicroz::compress_folder(const QString &source_path, const QString &zip_path)
{
    if (!QFileInfo::exists(source_path)) {
        qWarning() << "Folder not found:" << source_path;
        return false;
    }

    return compress_list({ source_path }, zip_path);
}

bool QMicroz::compress_list(const QStringList &paths, const QString &zip_path)
{
    if (paths.isEmpty())
        return false;

    const QString _root = QFileInfo(paths.first()).absolutePath();
    const QString _info = QStringLiteral(u"Zipping: ")
                          + (paths.size() == 1 ? paths.first() : (QString::number(paths.size()) + QStringLiteral(u" items")));
    qDebug() << _info;
    qDebug() << "Output:" << zip_path;

    // check if all paths are in the same root
    if (paths.size() > 1) {
        for (const QString &_path : paths) {
            if (_root != QFileInfo(_path).absolutePath()) {
                qWarning() << "ERROR: all items must be in the same folder!";
                return false;
            }
        }
    }

    QStringList _worklist;

    // parsing the path list
    for (const QString &_path : paths) {
        QFileInfo __fi(_path);
        if (!__fi.exists() || __fi.isSymLink()) {
            qWarning() << "Skipped:" << _path;
            continue;
        }

        if (__fi.isFile())
            _worklist << _path;
        else
            _worklist << tools::folderContent(_path);
    }

    return tools::createArchive(zip_path, _worklist, _root);
}

bool QMicroz::compress_buf(const BufFileList &buf_data, const QString &zip_path)
{
    qDebug() << "Zipping buffered data to:" << zip_path;

    if (buf_data.isEmpty()) {
        qDebug() << "No input data. Nothing to zip.";
        return false;
    }

    // create and open the output zip file
    mz_zip_archive *__za = tools::za_new(zip_path, tools::ZaWriter);
    if (!__za) {
        return false;
    }

    // process
    BufFileList::const_iterator it;
    for (it = buf_data.constBegin(); it != buf_data.constEnd(); ++it) {
        if (!tools::add_item_data(__za, it.key(), it.value())) {
            tools::za_close(__za);
            return false;
        }
    }

    // cleanup
    mz_zip_writer_finalize_archive(__za);
    tools::za_close(__za);
    qDebug() << "Done";
    return true;
}

bool QMicroz::compress_buf(const QByteArray &data, const QString &file_name, const QString &zip_path)
{
    BufFileList _buf;
    _buf[file_name] = data;
    return compress_buf(_buf, zip_path);
}

int QMicroz::findIndex(const QString &file_name)
{
    if (m_zip_contents.isEmpty())
        updateZipContents();

    ZipContentsList::const_iterator it;

    // full path matching
    for (it = m_zip_contents.constBegin(); it != m_zip_contents.constEnd(); ++it) {
        if (file_name == it.value())
            return it.key();
    }

    // deep search, matching only the name
    if (!file_name.contains(tools::s_sep)) {
        for (it = m_zip_contents.constBegin(); it != m_zip_contents.constEnd(); ++it) {
            if (!it.value().endsWith('/') // if not a subfolder
                && file_name == QFileInfo(it.value()).fileName())
            {
                return it.key();
            }
        }
    }

    qDebug() << "Index not found:" << file_name;
    return -1;
}
