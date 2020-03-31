#ifndef FILEOP_HPP_
#define FILEOP_HPP_

#include "global.hpp"

// Linux file operation 
#define S_struct_statfs  struct statfs64
#define S_statfs         statfs64
#define S_fstatfs        fstatfs64
#define S_struct_statvfs struct statvfs64
#define S_statvfs        statvfs64
#define S_fstatvfs       fstatvfs64
#define S_struct_stat    struct stat64
#define S_struct_flock   struct flock64
#define S_stat           stat64
#define S_lstat          lstat64
#define S_fstat          fstat64
#define S_open           open64
#define S_lseek          lseek64
#define S_ftruncate      ftruncate64
#define S_off_t          off64_t
#define S_close          close
#define S_access         access
#define S_chmod          chmod
#define S_read           read
#define S_write          write


// Linux file option
#define S_FILE_OP_FWRITE_BUF_SIZE 2048
#define S_FILE_OP_READ_ONLY     (((unsigned int)1) << 1)
#define S_FILE_OP_WRITE_ONLY    (((unsigned int)1) << 2)
#define S_FILE_OP_OPEN_EXISTING (((unsigned int)1) << 3)
#define S_FILE_OP_OPEN_ALWAYS   (((unsigned int)1) << 4)
#define S_FILE_OP_OPEN_TRUNC    (((unsigned int)1) << 5)

#define S_INVALID_HANDLE_FD_VALUE (-1)
#define S_OK 0


// type
typedef S_off_t file_offset;
typedef int fileHandle;

class fileOp
{
private:
    fileHandle _fileHandle;
    bool _isStdout;

    const fileOp &operator= (const fileOp &);

protected:
    void setFileHandle(fileHandle handle);

public:
    fileOp();
    bool isValid();

    // open and close
    int Open
    (
        const char * path,
        unsigned int options = S_FILE_OP_OPEN_ALWAYS
    );
    void openStdout();
    void Close();


    // seek and size
    file_offset getCurrentOffset() const;

    void seekToOffset(file_offset offset);

    void seekToEnd();

    file_offset getSize();

    // byte operation
    int readAt
    (
        const size_t offset,
        const size_t size, 
        void * buffer,
        int * const bytes
    );

    // handle
    fileHandle getHandle() const
    {
        return _fileHandle;
    } 
} ;



fileOp::fileOp()
{
    _fileHandle = S_INVALID_HANDLE_FD_VALUE;
    _isStdout = false;
}

// set the file handle
void fileOp::setFileHandle(fileHandle handle)
{
    _fileHandle = handle;
}

// check the validation of the file handle
bool fileOp::isValid()
{
    return (_fileHandle != S_INVALID_HANDLE_FD_VALUE);
}

// open a file and get the file handle
int fileOp::Open(
                const char * path,
                unsigned int options
                )
{
    int rc = S_OK;
    int mode = O_RDWR; 

    // get the mode from options
    if(options & S_FILE_OP_READ_ONLY) {
        mode = O_RDONLY;
    } else if (options & S_FILE_OP_WRITE_ONLY) {
        mode = O_WRONLY;
    }

    if(options & S_FILE_OP_OPEN_ALWAYS) {
        mode |= O_CREAT;
    }

    if(options & S_FILE_OP_OPEN_TRUNC) {
        mode |= O_TRUNC;
    }

    // keep trying to open the file when
    // interrupt makes the opening fail
    do {
        _fileHandle = S_open(path, mode, 0644);
    } while(
            (_fileHandle == -1) &&
            (errno == EINTR)
        );

    // fail to open the file
    if(_fileHandle <= S_INVALID_HANDLE_FD_VALUE){
        rc = errno;
        goto error;
    }
    goto done;

done:
    return rc;
error:
    goto done;
}

// make the stdout as the file handle
void fileOp::openStdout()
{
    setFileHandle(STDOUT_FILENO);
    _isStdout = true;
}

// close the files
void fileOp::Close()
{
    if(isValid() && (! _isStdout)) {
        S_close(_fileHandle);
        _fileHandle = S_INVALID_HANDLE_FD_VALUE;
    }
}




// get the cursor
file_offset fileOp::getCurrentOffset() const 
{
    return S_lseek(_fileHandle, 0, SEEK_CUR);
}

// set the cursor
void fileOp::seekToOffset(file_offset offset)
{
    if((file_offset) - 1 != offset) {
        S_lseek(_fileHandle, offset, SEEK_SET);
    }
}

// set the cursor at the end
void fileOp::seekToEnd()
{
   S_lseek( _fileHandle, 0, SEEK_END ) ;
}

// get the file size
file_offset fileOp::getSize()
{
    file_offset size = 0;
    S_struct_stat buf = {0} ; // stat64

    if(S_fstat(_fileHandle, &buf) == -1) {
        goto error;
    }

    size = buf.st_size;

done:
    return size;
error:
    size = 0;
    goto done;
}

int fileOp::readAt
                (
                    const size_t offset,
                    const size_t size, 
                    void * buffer,
                    int * const bytes
                )
{
    int rc = S_OK;
    ssize_t bytesRead = 0;

    if(isValid()) {
        seekToOffset(offset);
        do {
            bytesRead = S_read(_fileHandle, buffer, size);
        } while(
            (_fileHandle == -1) &&
            (errno == EINTR)
        );
    } else {
        goto error;
    }

    if(bytes) {
        *bytes = bytesRead;
    }

done:
    return rc;
error:
    *bytes = 0;
    rc = errno;
    goto done;
}



#endif