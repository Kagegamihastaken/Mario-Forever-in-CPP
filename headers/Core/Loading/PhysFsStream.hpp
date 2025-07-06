#ifndef PHYSFSSTREAM_HPP
#define PHYSFSSTREAM_HPP

#pragma once
#include <SFML/System/InputStream.hpp>
#include <physfs.h>
#include <string>

#include "soloud_file.h"

class PhysFsStream final : public sf::InputStream
{
public:
    explicit PhysFsStream(const char * filename = nullptr);
    ~PhysFsStream() override;
    bool isOpen() const;
    bool open(const char * filename);
    void close();

    std::optional<std::size_t> read(void * data, std::size_t size) override;
    std::optional<std::size_t> seek(std::size_t position) override;
    std::optional<std::size_t> tell() override;
    std::optional<std::size_t> getSize() override;

private:
    PHYSFS_File * m_File;
};

class PhysFsSoLoudStream final : public SoLoud::File {
public:
    PhysFsSoLoudStream();
    ~PhysFsSoLoudStream() override;
    bool open(const char *aFilename);
    unsigned int read(unsigned char *aDst, unsigned int aBytes) override;
    void seek(int aOffset) override;
    unsigned int pos() override;
    unsigned int length() override;
    int eof() override;
    void close();

private:
    PHYSFS_File* mFileHandle;
    std::string mFileName;
};



#endif //PHYSFSSTREAM_HPP
