#include "global.hpp"
#include "lock.hpp"
#include "fileOp.hpp"
#include "fileManager.hpp"

extern int SOLVER_NUMBER;

size_t problemNumber = 0;
extern std::queue<char *> problemBuffer[SOLVER_NUMBER_MAX];
extern cpLock problemBufferLock[SOLVER_NUMBER_MAX];
std::atomic<bool> readFile(true);

// no concurrent 
inline void problemWriter(char * problem)
{
    problemNumber++;
    unsigned int id = problemNumber % SOLVER_NUMBER;
    // printf("%s\n")

    problemBufferLock[id].pGet();
    problemBuffer[id].push(problem);
    problemBufferLock[id].pRelease();
}

void eofIntoProblemBuffer()
{
    for(unsigned int i = 0; i < SOLVER_NUMBER; i++)
    {
        problemWriter(NULL);
    }
}

exclusiveLock filenameLock;
std::queue<std::string> filenameBuffer;
extern unsigned int BLOCK_SIZE; //read a BLOCK from file every time

void * problemReadThread(void * arg)
{
    fileOp file;
    char * lineBuf = (char *) malloc(BATCH_NUMBER * 82);

    while(1)
    {
        // get the file 
        filenameLock.get();

        if(filenameBuffer.size() == 0)
        {
            // no file anymore
            if(readFile == false) {
                eofIntoProblemBuffer();
                filenameLock.release();
                break;
            }
            else {
                filenameLock.release();
                continue;
            }
        }

        std::string file_path = filenameBuffer.front();
        filenameBuffer.pop();

        filenameLock.release();

        
        size_t pos = 0;
        size_t lastSize = 0;
        
        // open the file
        file.Open(file_path.c_str(), S_FILE_OP_READ_ONLY);
        lastSize = file.getSize() - 1;

        
        // write in to the buf with batch size
        while((pos + 82 * BATCH_NUMBER - 1) <= lastSize) {

            // read in to the buf
            file.readAt(pos, 82 * BATCH_NUMBER - 1, lineBuf, NULL);

            // get it in to the  problem buffer
            for(int i = 0; i < BATCH_NUMBER; i++)
            {
                char * singleLine = (char *) malloc(82);
                strncpy(singleLine, &lineBuf[i * 82], 81);
                singleLine[81] = '\0';
                problemWriter(singleLine);
            }

            pos += 82 * BATCH_NUMBER;
        }

        size_t len = 0;
        if(pos <= lastSize) {
            len = ((lastSize - pos + 1) / 82) * 82;
        }

        // more file to read
        if(len)
        {
            int readSize = 0;
            file.readAt(pos, lastSize - pos + 1, lineBuf, &readSize);
            readSize = (readSize + 1) / 82 ;
            for(int i = 0; i < readSize; i++) 
            {
                char * singleLine = (char *) malloc(82);
                strncpy(singleLine, &lineBuf[i * 82], 81);
                singleLine[81] = '\0';
                problemWriter(singleLine);
            }
        }
        
        file.Close();
        
    }
    free(lineBuf);
}

void * filenameReadThread(void * arg){
    std::string file_path;

    while(std::cin >> file_path)
    {
        filenameLock.get();
        filenameBuffer.push(file_path);
        filenameLock.release();
    }

    readFile = false;
}
