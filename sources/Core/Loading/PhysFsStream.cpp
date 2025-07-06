#include "Core/Loading/PhysFsStream.hpp"
#include <physfs.h>
#include <stdexcept>
#include <fmt/format.h>

#include "Core/Logging.hpp"

PhysFsStream::PhysFsStream(const char * filename) : m_File(nullptr)
{
    if(filename)
        open(filename);
}

PhysFsStream::~PhysFsStream()
{
    close();
}

bool PhysFsStream::isOpen() const
{
    return (m_File != nullptr);
}

bool PhysFsStream::open(const char * filename)
{
    close();
    m_File = PHYSFS_openRead(filename);
    return isOpen();
}

void PhysFsStream::close()
{
    if(isOpen())
        PHYSFS_close(m_File);
    m_File = nullptr;
}

std::optional<std::size_t> PhysFsStream::read(void * data, std::size_t size)
{
    if(!isOpen())
        return -1;

    // PHYSFS_readBytes returns the number of bytes read or -1 on error.
    // We request to read size amount of bytes.
    return PHYSFS_readBytes(m_File, data, static_cast<PHYSFS_uint32>(size));
}

std::optional<std::size_t> PhysFsStream::seek(std::size_t position)
{
    if(!isOpen())
        return -1;

    // PHYSFS_seek return 0 on error and non zero on success
    if(PHYSFS_seek(m_File, position))
        return tell();
    else
        return -1;
}

std::optional<std::size_t> PhysFsStream::tell()
{
    if(!isOpen())
        return -1;

    // PHYSFS_tell returns the offset in bytes or -1 on error just like SFML wants.
    return PHYSFS_tell(m_File);
}

std::optional<std::size_t> PhysFsStream::getSize()
{
    if(!isOpen())
        return -1;

    // PHYSFS_fileLength also the returns length of file or -1 on error just like SFML wants.
    return PHYSFS_fileLength(m_File);
}

PhysFsSoLoudStream::PhysFsSoLoudStream() : mFileHandle(nullptr) {}
PhysFsSoLoudStream::~PhysFsSoLoudStream() {
    close();
}
bool PhysFsSoLoudStream::open(const char *aFilename) {
    close();
    mFileName = aFilename;
    mFileHandle = PHYSFS_openRead(aFilename);
    if (!mFileHandle)
        return false;
    return true;
        //throw std::runtime_error(fmt::format("PhysFsSoLoud: Cannot open file: ", aFilename));
}
unsigned int PhysFsSoLoudStream::read(unsigned char *aDst, const unsigned int aBytes) {
    if (!mFileHandle) return 0;
    PHYSFS_sint64 bytesRead = PHYSFS_read(mFileHandle, aDst, 1, aBytes);
    if (bytesRead == -1)
        return 0;
    return static_cast<unsigned int>(bytesRead);
}
void PhysFsSoLoudStream::seek(const int aOffset) {
    if (!mFileHandle) return;

    if (PHYSFS_seek(mFileHandle, aOffset) == 0)
        MFCPP::Log::ErrorPrint(fmt::format("PhysFS Seek Error: ", PHYSFS_getLastError()));
}
unsigned int PhysFsSoLoudStream::pos() {
    if (!mFileHandle) return 0;
    return static_cast<unsigned>(PHYSFS_tell(mFileHandle));
}
unsigned int PhysFsSoLoudStream::length() {
    if (!mFileHandle) return 0;
    return static_cast<unsigned>(PHYSFS_fileLength(mFileHandle));
}
int PhysFsSoLoudStream::eof() {
    if (!mFileHandle) return 1; // Treat as EOF if not open
    return PHYSFS_eof(mFileHandle);
}
void PhysFsSoLoudStream::close() {
    if (mFileHandle) {
        PHYSFS_close(mFileHandle);
        mFileHandle = nullptr;
    }
}






