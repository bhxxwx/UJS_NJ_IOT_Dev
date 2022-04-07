/*
 * Queue.h
 *
 *  Created on: 2020年5月05日
 *      Author: wxujs
 */

#include <stdio.h>
#include <string.h>
#include "Queue.h"



/**
 * 队列建立
 * @param pQ8 队列数据结构体指针
 * @param pBuf 队列缓冲区地址
 * @param bufSize 队列缓冲区大小
 * @return 固定值0
 */
uint32_t QUEUE_PacketCreate(QUEUE8_t *pQ8, uint8_t *pBuf, uint32_t bufSize)
{
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pBuf);
    ASSERT_PARAM(bufSize);

    pQ8->bufSize    = bufSize;
    pQ8->pBuf       = pBuf;
    pQ8->pStart     = pBuf;
    pQ8->pEnd       = pBuf;

    return 0;
}



/**
 * 数据入队列
 * @param pQ8 队列数据结构体指针
 * @param pData 要进队列的数据地址
 * @param len 入队列的数据长度
 * @return 入队列的数据个数
 */
uint32_t QUEUE_PacketIn(QUEUE8_t *pQ8, uint8_t *pData, uint32_t len)
{
    volatile uint8_t    *pEnd   = NULL;
    
    uint32_t            index   = 0;
    
    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pEnd = pQ8->pEnd;
    
    for (index = 0; index < len; index++)
    {
        if (++pEnd >= pQ8->pBuf + pQ8->bufSize)
        {
            pEnd = pQ8->pBuf;
        }
        if (pEnd == pQ8->pStart) 
        {
            break;
        }
        
        *pQ8->pEnd = *pData++;
        
        pQ8->pEnd = pEnd;
    }
    
    return index;
}


/**
 * 队列中取数据
 * @param pQ8 队列数据结构体指针
 * @param pData 要出队列数据存放的缓冲区地址
 * @param dataLen 要出队列的数据存放的缓冲区大小
 * @return 实际出队列的数据个数
 */
uint32_t QUEUE_PacketOut(QUEUE8_t *pQ8, uint8_t *pData, uint32_t dataLen)
{
    uint32_t index = 0;
    
    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    while ((pQ8->pStart != pQ8->pEnd) && (index < dataLen) && (index < pQ8->bufSize))
    {
        pData[index++] = *pQ8->pStart++;
        if (pQ8->pStart >= pQ8->pBuf + pQ8->bufSize) 
        {
            pQ8->pStart = pQ8->pBuf;
        }
    }

    return index;
}


/**
 * 获取队列中的数据长度
 * @param pQ8 队列数据结构体指针
 * @return 当前队列中的数据长度
 */
uint32_t QUEUE_PacketLengthGet(QUEUE8_t *pQ8)
{
    
    volatile uint8_t    *pStart     = NULL;
    uint32_t            index       = 0;
    
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart = pQ8->pStart;

    while ((pStart != pQ8->pEnd) && (index < pQ8->bufSize))
    {
        index++;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) 
        {
            pStart = pQ8->pBuf;
        }
    }

    return index;
}


/**
 * 以起始符和结束符取队列中的数据 (取出的数据 包括起始符 和结束符)
 * @param pQ8 队列数据结构体指针
 * @param startChar 起始字符
 * @param endChar 结束字符
 * @param pData 存放取出数据的缓冲区地址
 * @param dataLen 存放取出数据的缓冲区的大小
 * @return 实际取出的数据长度
 */
uint32_t QUEUE_PacketStartEndDifferentCharSplit(QUEUE8_t *pQ8, uint8_t startChar, uint8_t endChar, uint8_t *pData, uint32_t dataLen)
{
    int32_t count;
    int32_t index;
    volatile uint8_t *pStart;
    volatile uint8_t *pEnd;
    
    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart      = pQ8->pStart;
    count       = pQ8->bufSize;
    
    while ((pStart != pQ8->pEnd) && count--)        //查找起始字符
    {
        if (startChar == *pStart) break;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    if (pStart == pQ8->pEnd) return 0;              //未找到起始符
    if (count == -1) return 0;
    pEnd = pStart;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;

    while ((pEnd != pQ8->pEnd) && count--)          //查找结束字符
    {
        if (endChar == *pEnd) break;
        if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
    }
    
    if (pEnd == pQ8->pEnd) return 0;                //未找结束符
    if (count == -1) return 0;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
    
    count   = pQ8->bufSize - count;   
    index   = 0;
    //获取从起始字符到结束字符的数据
    while ((pStart != pEnd) && (index < dataLen) && (index < pQ8->bufSize) && count--)
    {
        pData[index++] = *pStart++;
        if (pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    pQ8->pStart = pEnd;
    return index;
}


/**
 * 提取首尾分隔符内的数据(包括分隔符)
 * @param pQ8 队列数据结构体指针
 * @param splitChar 分隔符
 * @param pData 存放取出数据的缓冲区地址
 * @param dataLen 存放取出数据的缓冲区的大小
 * @return 实际取出的数据长度
 */
uint32_t QUEUE_PacketStartEndCharSplit(QUEUE8_t *pQ8, uint8_t splitChar, uint8_t *pData, uint32_t dataLen)
{
    int32_t count;
    int32_t index;
    volatile uint8_t *pStart;
    volatile uint8_t *pEnd;
    
    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart      = pQ8->pStart;
    count       = pQ8->bufSize;
    
    while ((pStart != pQ8->pEnd) && count--)        //查找起始字符
    {
        if (splitChar == *pStart) break;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    if (pStart == pQ8->pEnd) return 0;              //未找到起始符
    if (count == -1) return 0;
    pEnd = pStart;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;

    while ((pEnd != pQ8->pEnd) && count--)          //查找结束字符
    {
        if (splitChar == *pEnd) break;
        if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
    }
    
    if (pEnd == pQ8->pEnd) return 0;                //未找结束符
    if (count == -1) return 0;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
    
    count   = pQ8->bufSize - count;   
    index   = 0;
    //获取从起始字符到结束字符的数据
    while ((pStart != pEnd) && (index < dataLen) && (index < pQ8->bufSize) && count--)
    {
        pData[index++] = *pStart++;
        if (pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    //如果取出的数据只包括分隔符，有可能是上次结束符和下次起始符，因此放弃上次结束符。
    if (index <= 2)                                  
    {
        index = 0;
        if (--pStart < pQ8->pBuf) pStart = pQ8->pBuf + pQ8->bufSize - 1;
    }

    pQ8->pStart = pStart;
    return index;
}


/**
 * 提取单结束分隔符的数据 (包括分隔符)
 * @param pQ8 队列数据结构体指针
 * @param splitChar 分隔符
 * @param pData 存放取出数据的缓冲区地址
 * @param dataLen 存放取出数据的缓冲区的大小
 * @return 实际取出的数据长度
 */
uint32_t QUEUE_PacketCharSplit(QUEUE8_t *pQ8, uint8_t splitChar, uint8_t *pData, uint32_t dataLen)
{
    int32_t count;
    int32_t index;
    volatile uint8_t *pStart;
    volatile uint8_t *pEnd;
    
    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart      = pQ8->pStart;
    count       = pQ8->bufSize;

    while ((pStart != pQ8->pEnd) && count--)        //查找起始字符
    {
        if (splitChar == *pStart) break;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    if (pStart == pQ8->pEnd) return 0;              //未找到起始符
    if (count == -1) return 0;
    pEnd = pStart;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
    
    pStart      = pQ8->pStart;
    count       = pQ8->bufSize;
    index       = 0;
    while ((pStart != pEnd) && (index < dataLen) && count--)        //查找起始字符
    {
        pData[index++] = *pStart;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }
    
    pQ8->pStart = pStart;
    return index;
}

/**
 * 提取双结束分隔符的数据 (包括分隔符)
 * @param pQ8 队列数据结构体指针
 * @param splitChar1 分隔符1
 * @param splitChar2 分隔符2
 * @param pData 存放取出数据的缓冲区地址
 * @param dataLen 存放取出数据的缓冲区的大小
 * @return 实际取出的数据长度
 */
uint32_t QUEUE_PacketDoubleEndCharSplit(QUEUE8_t *pQ8, uint8_t splitChar1, uint8_t splitChar2, uint8_t *pData, uint32_t dataLen)
{
    int32_t count;
    int32_t index;
    volatile uint8_t *pStart;
    volatile uint8_t *pEnd;
    uint8_t lastChar = 0;

    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart      = pQ8->pStart;
    count       = pQ8->bufSize;

    while ((pStart != pQ8->pEnd) && count--)        //查找起始字符
    {
    	//当前字符串==splitchar2且上一个字符串==splitchar1则退出
        if ((splitChar1 == lastChar) && (splitChar2 == *pStart)) break;

        lastChar = *pStart;

        //当前指针在缓冲区最后一个,下一个应该在缓冲区第一个
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    if (pStart == pQ8->pEnd) return 0;              //未找到起始符
    if (count == -1) return 0;
    pEnd = pStart;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;

    pStart      = pQ8->pStart;
    count       = pQ8->bufSize;
    index       = 0;
    while ((pStart != pEnd) && (index < dataLen) && count--)        //拷贝数据
    {
        pData[index++] = *pStart;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    pQ8->pStart = pStart;
    return index;
}





/**
 * 结构体栈初始化,创建结构体栈的数据结构体
 * @param pQueue 结构体栈的数据结构体地址
 * @param pBuf 栈缓冲区
 * @param bufSize 换冲区大小(字节)
 * @param blkSize 单结构体大小(字节)
 * @return 固定值0
 */
uint32_t QUEUE_StructCreate(QUEUE_STRUCT_t *pQueue, void *pBuf, uint32_t bufSize, uint16_t blkSize)
{
    ASSERT_PARAM(pQueue);
    ASSERT_PARAM(pBuf);
    ASSERT_PARAM(bufSize);
    ASSERT_PARAM(blkSize);

    pQueue->elemSize    = blkSize;
    pQueue->sumCount    = bufSize / blkSize;
    pQueue->pBuf        = pBuf;
    pQueue->start       = 0;
    pQueue->end         = 0;
    return 0;
}

/**
 * 结构体入栈,缓冲区中如果满则不载入
 * @param pQueue 结构体栈的数据结构体地址
 * @param pData 如队列数据地址
 * @param blkCount 准备入栈的结构体个数(单位:结构体个数)
 * @return 成功入栈结构体的个数
 */
uint32_t QUEUE_StructIn(QUEUE_STRUCT_t *pQueue, void *pData, uint32_t blkCount)
{
    uint32_t i = blkCount;
    uint32_t end = 0;
    
    ASSERT_PARAM(pQueue);
    ASSERT_PARAM(pQueue->pBuf);
    ASSERT_PARAM(pData);

    end = pQueue->end;
    for (i = 0; i < blkCount; i++)
    {
        //再装一组数据后，指针是否指向栈尾
        if (++end >= pQueue->sumCount)      
        {
            end = 0;
        }
        
        //缓冲区填满 直接退出
        if (end == pQueue->start)   
        {
            break; 
        }
        
        memcpy((uint8_t *)pQueue->pBuf + pQueue->end * pQueue->elemSize, pData, pQueue->elemSize);

        pData = (uint8_t *)pData + pQueue->elemSize;
        pQueue->end = end;
    }
    
    return i;
}


/**
 * 结构体出栈
 * @param pQueue 结构体队列的数据结构体地址
 * @param pData 如队列数据地址
 * @param blkCount 准备入栈的结构体个数(单位:结构体个数)
 * @return 成功出栈结构体的个数
 */
uint32_t QUEUE_StructOut(QUEUE_STRUCT_t *pQueue, void *pData, uint32_t blkCount)
{
    uint32_t index = 0;
    
    ASSERT_PARAM(pQueue);
    ASSERT_PARAM(pQueue->pBuf);
    ASSERT_PARAM(pData);

    while ((pQueue->start != pQueue->end) && (index < pQueue->sumCount) && (index < blkCount))
    {
        memcpy(pData, (uint8_t *)pQueue->pBuf + pQueue->start * pQueue->elemSize, pQueue->elemSize);

        pData = (uint8_t *)pData + pQueue->elemSize;
        index++;
        if (++pQueue->start >= pQueue->sumCount) 
        {
            pQueue->start = 0;
        }
    }

    return index;
}

/**
 * 获取结构体队列中的个数(结构体个数)
 * @param pQueue 结构体队列的数据结构体地址
 * @return
 */
uint32_t QUEUE_StructCountGet(QUEUE_STRUCT_t *pQueue)
{
    uint32_t index = 0;
    uint32_t start =0;
    
    ASSERT_PARAM(pQueue);
    ASSERT_PARAM(pQueue->pBuf);

    start = pQueue->start;
    while ((start != pQueue->end) && (index < pQueue->sumCount))
    {
        index++;
        if (++start >= pQueue->sumCount) 
        {
            start = 0;
        }
    }

    return index;
}


#if     DEBUG_FULL_ASSERT

/**
 * 断言异常
 */
void ASSERT_FAILED(uint8_t* file, uint32_t line)
{
    uint8_t flg = 1;

    printf("wrong information 文件:%s 第%d行\r\n", file, line);
    while (flg);
}

#endif

