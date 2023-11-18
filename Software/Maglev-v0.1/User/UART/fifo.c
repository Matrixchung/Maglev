//******************************************************************************************
//!
//! \file   FIFO.c
//! \brief  Genernal FIFO Model Interface.
//!         You can use uniform FIFO Model to manager Any type of data element.
//! \author cedar
//! \changed damom.li
//! \changed sky.huang
//! \date   2016-11-03//! \date   2013-12-16
//! \email  xuesong5825718@gmail.com
//!
//! \version: 1.1 add fifo_s_gets_noprotect function
//!
//! Copyright (c) 2013 Cedar MIT License
//!
//! Permission is hereby granted, free of charge, to any person obtaining a copy
//! of this software and associated documentation files (the "Software"), to deal
//! in the Software without restriction, including without limitation the rights to
//! use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
//! the Software, and to permit persons to whom the Software is furnished to do so,
//! subject to the following conditions:
//!
//! The above copyright notice and this permission notice shall be included in all
//! copies or substantial portions of the Software.
//!
//! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//! IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//! FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//! AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//! LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//! OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//! IN THE SOFTWARE.
///
//******************************************************************************************
#include "fifo.h"

//******************************************************************************************
//!                     ASSERT MACRO
//******************************************************************************************
#ifndef ASSERT

#ifdef FIFO_NDEBUG
#define ASSERT(x)
#else
#define ASSERT(x)                                           \
    do                                                      \
    {                                                       \
        if (!(x))                                           \
            printf("[assert]: %s, %d", __FILE__, __LINE__); \
        while (!(x))                                        \
            ;                                               \
    } while (0)
#endif

#endif // ASSERT

#ifdef USE_DYNAMIC_MEMORY
//******************************************************************************************
//
//! \brief  Create An New FIFO Instance(in Single Mode).
//! This function allocate enought room for N blocks fifo elements, then return the pointer
//! of FIFO.
//!
//! \param  [in] uint_cnt is count of fifo elements.
//! \retval The Pointer of FIFO instance, return NULL is failure to allocate memory.
//!
//! \note   -# You must enable USE_MEMORY_ALLOC macro and ensure your system have <stdlib.h>
//!            Header file before use this function.
//! \note   -# Functions FIFO_Create and FIFO_Destory must be used in pairs.
//!
//******************************************************************************************
fifo_s_t *fifo_s_create(uint16_t uint_cnt)
{
    fifo_s_t *p_fifo = NULL;  //!< FIFO Pointer
    char *p_base_addr = NULL; //!< Memory Base Address

    //! Check input parameters.
    ASSERT(uint_cnt);

    //! Allocate Memory for pointer of new FIFO Control Block
    p_fifo = (fifo_s_t *)MEMALLOC(sizeof(fifo_s_t));
    if (NULL == p_fifo)
    {
        //! Allocate Failure, exit now
        return (NULL);
    }
    //! Allocate Memory for pointer of new FIFO
    p_base_addr = MEMALLOC(uint_cnt);
    if (NULL == p_base_addr)
    {
        //! Allocate Failure, exit now
        MEMFREE(p_fifo);
        return (NULL);
    }
    //! Initialize General FIFO Module
    fifo_s_init(p_fifo, p_base_addr, uint_cnt);

    return (p_fifo);
}

//******************************************************************************************
//
//! \brief  Destory FIFO Instance(in Single Mode).
//!  This function release memory, then reinit the FIFO struct.
//!
//! \param  [in] p_fifo is the pointer of FIFO instance
//! \retval None.
//!
//! \note   -# You must enable USE_MEMORY_ALLOC macro and ensure your system have <stdlib.h>
//!            Header file before use this function.
//
//******************************************************************************************
void fifo_s_destroy(fifo_s_t *p_fifo)
{
    //! Check input parameters.
    ASSERT(p_fifo);
    ASSERT(p_fifo->p_start_addr);

    //! Free FIFO memory
    free(p_fifo->p_start_addr);
    //! Free FIFO Control Block memory
    free(p_fifo);

    return; //!< Success
}

#endif // USE_DYNAMIC_MEMORY

//******************************************************************************************
//
//! \brief  Initialize an static FIFO struct(in single mode).
//!
//! \param  [in] p_fifo is the pointer of valid FIFO instance.
//! \param  [in] p_base_addr is the base address of pre-allocate memory, such as array.
//! \param  [in] uint_cnt is count of fifo elements.
//! \retval 0 if initialize successfully, otherwise return -1.
//
//******************************************************************************************
char fifo_s_init(fifo_s_t *p_fifo, void *p_base_addr, uint16_t uint_cnt)
{
    //! Check input parameters.
    ASSERT(p_fifo);
    ASSERT(p_base_addr);
    ASSERT(uint_cnt);

    //! Initialize FIFO Control Block.
    p_fifo->p_start_addr = (char *)p_base_addr;
    p_fifo->p_end_addr = (char *)p_base_addr + uint_cnt - 1;
    p_fifo->free_num = uint_cnt;
    p_fifo->used_num = 0;
    p_fifo->p_read_addr = (char *)p_base_addr;
    p_fifo->p_write_addr = (char *)p_base_addr;

    //! Inint mutex for new FIFO
    MUTEX_INIT(p_fifo->mutex);

    return (0);
}

//******************************************************************************************
//
//! \brief  Put an element into FIFO(in single mode).
//!
//! \param  [in]  p_fifo is the pointer of valid FIFO.
//! \param  [in]  element is the data element you want to put
//!
//! \retval 0 if operate successfully, otherwise return -1.
//
//******************************************************************************************
char fifo_s_put(fifo_s_t *p_fifo, char element)
{
    //! Check input parameters.
    ASSERT(p_fifo);

    if (0 == p_fifo->free_num)
    {
        //! Error, FIFO is full!
        return (-1);
    }

    MUTEX_LOCK(p_fifo->mutex);

    if (0 == p_fifo->free_num)
    {
        //! Error, FIFO is full!
        MUTEX_UNLOCK(p_fifo->mutex);
        return (-1);
    }

    if (p_fifo->p_write_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_write_addr = p_fifo->p_start_addr;
    }

    *(p_fifo->p_write_addr) = element;
    p_fifo->p_write_addr++;
    p_fifo->free_num--;
    p_fifo->used_num++;
    MUTEX_UNLOCK(p_fifo->mutex);

    return (0);
}

//******************************************************************************************
//
//! \brief  Put some elements into FIFO(in single mode).
//!
//! \param  [in]  p_fifo is the pointer of valid FIFO.
//! \param  [in]  p_source is the data element you want to put
//! \param  [in]  the number of elements
//! \retval the number of really write data, otherwise return -1.
//
//******************************************************************************************
int16_t fifo_s_puts(fifo_s_t *p_fifo, char *p_source, uint16_t len)
{
    int16_t len_to_end;
    int16_t len_from_start;

    ASSERT(p_fifo);

    if (NULL == p_source)
    {
        return -1;
    }

    if (0 == p_fifo->free_num)
    {
        return 0;
    }

    MUTEX_LOCK(p_fifo->mutex);

    if (0 == p_fifo->free_num)
    {
        MUTEX_UNLOCK(p_fifo->mutex);
        return 0;
    }

    if (p_fifo->p_write_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_write_addr = p_fifo->p_start_addr;
    }

    len = (len < p_fifo->free_num) ? len : p_fifo->free_num;
    len_to_end = p_fifo->p_end_addr - p_fifo->p_write_addr + 1;

    if (len_to_end >= len) //no rollback
    {
        len_to_end = len;
        memcpy(p_fifo->p_write_addr, p_source, len_to_end);
        p_fifo->p_write_addr += len_to_end;
    }
    else //rollback
    {
        len_from_start = len - len_to_end;
        memcpy(p_fifo->p_write_addr, p_source, len_to_end);
        memcpy(p_fifo->p_start_addr, p_source + len_to_end, len_from_start);
        p_fifo->p_write_addr = p_fifo->p_start_addr + len_from_start;
    }

    p_fifo->free_num -= len;
    p_fifo->used_num += len;
    MUTEX_UNLOCK(p_fifo->mutex);

    return len;
}

//******************************************************************************************
//
//! \brief  Put some elements into FIFO, ingnore the interrupt
//!
//! \param  [in]  p_fifo is the pointer of valid FIFO.
//! \param  [in]  p_source is the data element you want to put
//! \param  [in]  the number of elements
//! \retval the number of really write data, otherwise return -1.
//
//******************************************************************************************
int16_t fifo_s_puts_noprotect(fifo_s_t *p_fifo, char *p_source, uint16_t len)
{
    int16_t len_to_end;
    int16_t len_from_start;

    ASSERT(p_fifo);

    if (NULL == p_source)
    {
        return -1;
    }

    if (0 == p_fifo->free_num)
    {
        return 0;
    }

    if (p_fifo->p_write_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_write_addr = p_fifo->p_start_addr;
    }

    len = (len < p_fifo->free_num) ? len : p_fifo->free_num;
    len_to_end = p_fifo->p_end_addr - p_fifo->p_write_addr + 1;

    if (len_to_end >= len) //no rollback
    {
        len_to_end = len;
        memcpy(p_fifo->p_write_addr, p_source, len_to_end);
        p_fifo->p_write_addr += len_to_end;
    }
    else //rollback
    {
        len_from_start = len - len_to_end;
        memcpy(p_fifo->p_write_addr, p_source, len_to_end);
        memcpy(p_fifo->p_start_addr, p_source + len_to_end, len_from_start);
        p_fifo->p_write_addr = p_fifo->p_start_addr + len_from_start;
    }

    p_fifo->free_num -= len;
    p_fifo->used_num += len;

    return len;
}

//******************************************************************************************
//
//! \brief  Get an element from FIFO(in single mode).
//!
//! \param  [in]  p_fifo is the pointer of valid FIFO.
//!
//! \retval the data element of FIFO.
//
//******************************************************************************************
char fifo_s_get(fifo_s_t *p_fifo)
{
    char retval = 0;

    //! Check input parameters.
    ASSERT(p_fifo);

    //TODO:
    if (0 == p_fifo->used_num)
    {
        return 0;
    }

    MUTEX_LOCK(p_fifo->mutex);

    if (0 == p_fifo->used_num)
    {
        MUTEX_UNLOCK(p_fifo->mutex);
        return 0;
    }

    if (p_fifo->p_read_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_read_addr = p_fifo->p_start_addr;
    }

    retval = *p_fifo->p_read_addr;
    // Update information
    p_fifo->p_read_addr++;
    p_fifo->free_num++;
    p_fifo->used_num--;
    MUTEX_UNLOCK(p_fifo->mutex);

    return (retval);
}

//******************************************************************************************
//
//! \brief  Get some element from FIFO(in single mode).
//!
//! \param  [in]  p_fifo is the pointer of valid FIFO.
//!
//! \retval the number of really read data.
//
//******************************************************************************************
int16_t fifo_s_gets(fifo_s_t *p_fifo, char *p_dest, uint16_t len)
{
    int16_t len_to_end;
    int16_t len_from_start;

    ASSERT(p_fifo);

    if (NULL == p_dest)
    {
        return -1;
    }

    if (0 == p_fifo->used_num)
    {
        return 0;
    }

    MUTEX_LOCK(p_fifo->mutex);

    if (0 == p_fifo->used_num)
    {
        MUTEX_UNLOCK(p_fifo->mutex);
        return 0;
    }

    if (p_fifo->p_read_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_read_addr = p_fifo->p_start_addr;
    }

    len = (len < p_fifo->used_num) ? len : p_fifo->used_num;
    len_to_end = p_fifo->p_end_addr - p_fifo->p_read_addr + 1;

    if (len_to_end >= len) //no rollback
    {
        len_to_end = len;
        memcpy(p_dest, p_fifo->p_read_addr, len_to_end);
        p_fifo->p_read_addr += len_to_end;
    }
    else //rollback
    {
        len_from_start = len - len_to_end;
        memcpy(p_dest, p_fifo->p_read_addr, len_to_end);
        memcpy(p_dest + len_to_end, p_fifo->p_start_addr, len_from_start);
        p_fifo->p_read_addr = p_fifo->p_start_addr + len_from_start;
    }

    p_fifo->free_num += len;
    p_fifo->used_num -= len;
    MUTEX_UNLOCK(p_fifo->mutex);

    return len;
}

//******************************************************************************************
//
//! \brief  Get some element from FIFO(in single mode).
//!
//! \param  [in]  p_fifo is the pointer of valid FIFO.
//!
//! \retval the number of really read data.
//
//******************************************************************************************
int16_t fifo_s_gets_noprotect(fifo_s_t *p_fifo, char *p_dest, uint16_t len)
{
    int16_t len_to_end;
    int16_t len_from_start;

    ASSERT(p_fifo);

    if (NULL == p_dest)
    {
        return -1;
    }

    if (0 == p_fifo->used_num)
    {
        return 0;
    }

    if (p_fifo->p_read_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_read_addr = p_fifo->p_start_addr;
    }

    len = (len < p_fifo->used_num) ? len : p_fifo->used_num;
    len_to_end = p_fifo->p_end_addr - p_fifo->p_read_addr + 1;

    if (len_to_end >= len) //no rollback
    {
        len_to_end = len;
        memcpy(p_dest, p_fifo->p_read_addr, len_to_end);
        p_fifo->p_read_addr += len_to_end;
    }
    else //rollback
    {
        len_from_start = len - len_to_end;
        memcpy(p_dest, p_fifo->p_read_addr, len_to_end);
        memcpy(p_dest + len_to_end, p_fifo->p_start_addr, len_from_start);
        p_fifo->p_read_addr = p_fifo->p_start_addr + len_from_start;
    }

    p_fifo->free_num += len;
    p_fifo->used_num -= len;

    return len;
}

//******************************************************************************************
//
//! \brief  Pre-Read an element from FIFO(in single mode).
//!
//! \param  [in]  p_fifo is the pointer of valid FIFO.
//! \param  [in]  Offset is the offset from current pointer.
//!
//! \retval the data element of FIFO.
//
//******************************************************************************************
char fifo_s_preread(fifo_s_t *p_fifo, char offset)
{
    char *tmp_read_addr;

    //! Check input parameters.
    ASSERT(p_fifo);

    if (offset > p_fifo->used_num)
    {
        return 0;
    }
    else
    {
        // Move Read Pointer to right position
        tmp_read_addr = p_fifo->p_read_addr + offset;
        if (tmp_read_addr > p_fifo->p_end_addr)
        {
            tmp_read_addr = tmp_read_addr - p_fifo->p_end_addr + p_fifo->p_start_addr - 1;
        }

        return *tmp_read_addr;
    }
}

/*
 *
 *
 *
 *
 */
int16_t fifo_s_prereads(fifo_s_t *p_fifo, char *p_dest, char offset, uint16_t len)
{
    char *tmp_read_addr;
    int16_t len_to_end;
    int16_t len_from_start;

    ASSERT(p_fifo);

    if (NULL == p_dest)
    {
        return -1;
    }

    if (0 == p_fifo->used_num)
    {
        return -1;
    }

    if (offset >= p_fifo->used_num)
    {
        return -1;
    }

    MUTEX_LOCK(p_fifo->mutex);

    if (0 == p_fifo->used_num)
    {
        MUTEX_UNLOCK(p_fifo->mutex);
        return -1;
    }

    if (offset >= p_fifo->used_num)
    {
        MUTEX_UNLOCK(p_fifo->mutex);
        return -1;
    }

    tmp_read_addr = p_fifo->p_read_addr + offset;
    if (tmp_read_addr > p_fifo->p_end_addr)
    {
        tmp_read_addr = tmp_read_addr - p_fifo->p_end_addr + p_fifo->p_start_addr - 1;
    }

    len = (len < (p_fifo->used_num - offset)) ? len : (p_fifo->used_num - offset);
    len_to_end = p_fifo->p_end_addr - tmp_read_addr + 1;

    if (len_to_end >= len) //no rollback
    {
        len_to_end = len;
        memcpy(p_dest, tmp_read_addr, len_to_end);
    }
    else //rollback
    {
        len_from_start = len - len_to_end;
        memcpy(p_dest, tmp_read_addr, len_to_end);
        memcpy(p_dest + len_to_end, p_fifo->p_start_addr, len_from_start);
    }

    MUTEX_UNLOCK(p_fifo->mutex);

    return len;
}

//******************************************************************************************
//
//! \brief  FIFO is empty (in single mode)?
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval - None-zero(true) if empty.
//!         - Zero(false) if not empty.
//
//******************************************************************************************
char fifo_s_isempty(fifo_s_t *p_fifo)
{
    //! Check input parameter.
    ASSERT(p_fifo);
    return (p_fifo->used_num ? 0 : 1);
}

//******************************************************************************************
//
//! \brief  FIFO is full (in single mode)?
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval - None-zero(true) if full.
//!         - Zero(false) if not full.
//
//******************************************************************************************
char fifo_s_isfull(fifo_s_t *p_fifo)
{
    //! Check input parameter.
    ASSERT(p_fifo);
    return (p_fifo->free_num ? 0 : 1);
}

//******************************************************************************************
//
//! \brief  Get FIFO the number of elements(in single mode)?
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval The number of elements in FIFO.
//
//******************************************************************************************
uint16_t fifo_s_used(fifo_s_t *p_fifo)
{
    //! Check input parameter.
    ASSERT(p_fifo);
    return p_fifo->used_num;
}

//******************************************************************************************
//
//! \brief  Get FIFO the number of elements(in single mode)?
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval The number of elements in FIFO.
//
//******************************************************************************************
uint16_t fifo_s_free(fifo_s_t *p_fifo)
{
    //! Check input parameter.
    ASSERT(p_fifo);
    return p_fifo->free_num;
}

//******************************************************************************************
//
//! \brief  Flush the content of FIFO.
//!
//! \param  [in] p_fifo is the pointer of valid FIFO.
//!
//! \retval 0 if success, -1 if failure.
//
//******************************************************************************************
void fifo_s_flush(fifo_s_t *p_fifo)
{
    //! Check input parameters.
    ASSERT(p_fifo);
    //! Initialize FIFO Control Block.
    MUTEX_LOCK(p_fifo->mutex);
    p_fifo->free_num = p_fifo->p_end_addr - p_fifo->p_start_addr + 1;
    p_fifo->used_num = 0;
    p_fifo->p_read_addr = p_fifo->p_start_addr;
    p_fifo->p_write_addr = p_fifo->p_start_addr;
    MUTEX_UNLOCK(p_fifo->mutex);
}

uint16_t fifo_s_discard(fifo_s_t *p_fifo, uint16_t len)
{
    //! Check input parameters.
    char *tmp_index;
    ASSERT(p_fifo);

    MUTEX_LOCK(p_fifo->mutex);
    if (len > p_fifo->used_num)
    {
        len = p_fifo->used_num;
    }

    tmp_index = len + p_fifo->p_read_addr;
    if (tmp_index > p_fifo->p_end_addr)
    {
        tmp_index = tmp_index - p_fifo->p_end_addr + p_fifo->p_start_addr - 1;
    }
    p_fifo->p_read_addr = tmp_index;
    p_fifo->free_num += len;
    p_fifo->used_num -= len;
    MUTEX_UNLOCK(p_fifo->mutex);
    return len;
}

#ifdef USE_DYNAMIC_MEMORY
//******************************************************************************************
//
//! \brief  Create An New FIFO Instance.
//! This function allocate enought room for N blocks fifo elements, then return the pointer
//! of FIFO.
//!
//! \param  [in] UnitSize is fifo element size.
//! \param  [in] UnitCnt is count of fifo elements.
//! \retval The Pointer of FIFO instance, return NULL is failure to allocate memory.
//!
//! \note   -# You must enable USE_MEMORY_ALLOC macro and ensure your system have <stdlib.h>
//!            Header file before use this function.
//! \note   -# Functions FIFO_Create and FIFO_Destory must be used in pairs.
//!
//******************************************************************************************
fifo_t *fifo_create(char unit_size, uint16_t unit_cnt)
{
    fifo_t *p_fifo = NULL;    //!< FIFO Pointer
    char *p_base_addr = NULL; //!< Memory Base Address

    //! Check input parameters.
    ASSERT(unit_size);
    ASSERT(unit_cnt);

    //! Allocate Memory for pointer of new FIFO Control Block.
    p_fifo = (fifo_t *)MEMALLOC(sizeof(fifo_t));
    if (NULL == p_fifo)
    {
        //! Allocate Failure, exit now.
        return (NULL);
    }

    //! Allocate memory for FIFO.
    p_base_addr = MEMALLOC(unit_size * unit_cnt);
    if (NULL == p_base_addr)
    {
        //! Allocate Failure, exit now.
        MEMFREE(p_fifo);
        return (NULL);
    }

    //! Initialize General FIFO Module.
    fifo_init(p_fifo, p_base_addr, unit_size, unit_cnt);

    return (p_fifo);
}

//******************************************************************************************
//
//! \brief  Destory FIFO Instance.
//!  This function release memory, then reinit the FIFO struct.
//!
//! \param  [in] pFIFO is the pointer of FIFO instance
//! \retval None.
//!
//! \note   -# You must enable USE_MEMORY_ALLOC macro and ensure your system have <stdlib.h>
//!            Header file before use this function.
//
//******************************************************************************************
void fifo_destroy(fifo_t *p_fifo)
{
    //! Check input parameters.
    ASSERT(p_fifo);
    ASSERT(p_fifo->p_start_addr);

    //! Free FIFO memory
    free(p_fifo->p_start_addr);
    //! Free FIFO Control Block memory.
    free(p_fifo);

    return; //!< Success
}

#endif // USE_DYNAMIC_MEMORY

//******************************************************************************************
//
//! \brief  Initialize an static FIFO struct.
//!
//! \param  [in] pFIFO is the pointer of valid FIFO instance.
//! \param  [in] pBaseAddr is the base address of pre-allocate memory, such as array.
//! \param  [in] UnitSize is fifo element size.
//! \param  [in] UnitCnt is count of fifo elements.
//! \retval 0 if initialize successfully, otherwise return -1.
//
//******************************************************************************************
char fifo_init(fifo_t *p_fifo, void *p_base_addr, char unit_size, uint16_t unit_cnt)
{
    //! Check input parameters.
    ASSERT(p_fifo);
    ASSERT(p_base_addr);
    ASSERT(unit_size);
    ASSERT(unit_cnt);

    //! Initialize FIFO Control Block.
    p_fifo->p_start_addr = (char *)p_base_addr;
    p_fifo->p_end_addr = (char *)p_base_addr + unit_size * unit_cnt - 1;
    p_fifo->free_num = unit_cnt;
    p_fifo->used_num = 0;
    p_fifo->unit_size = unit_size;
    p_fifo->p_read_addr = (char *)p_base_addr;
    p_fifo->p_write_addr = (char *)p_base_addr;

    MUTEX_INIT(p_fifo->mutex);

    return (0);
}

//******************************************************************************************
//
//! \brief  Put an element into FIFO.
//!
//! \param  [in]  pFIFO is the pointer of valid FIFO.
//! \param  [in]  pElement is the address of element you want to put
//!
//! \retval 0 if operate successfully, otherwise return -1.
//
//******************************************************************************************
char fifo_put(fifo_t *p_fifo, void *p_element)
{
    //! Check input parameters.
    ASSERT(p_fifo);
    ASSERT(p_element);

    // Full ?
    if (0 == p_fifo->free_num)
    {
        //! Error, FIFO is full!
        return (-1);
    }

    //! Copy Data
    MUTEX_LOCK(p_fifo->mutex);

    if (0 == p_fifo->free_num)
    {
        //! Error, FIFO is full!
        MUTEX_UNLOCK(p_fifo->mutex);
        return (-1);
    }

    if (p_fifo->p_write_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_write_addr = p_fifo->p_start_addr;
    }

    memcpy(p_fifo->p_write_addr, p_element, p_fifo->unit_size);
    p_fifo->p_write_addr += p_fifo->unit_size;
    p_fifo->free_num--;
    p_fifo->used_num++;
    MUTEX_UNLOCK(p_fifo->mutex);

    return (0);
}

//******************************************************************************************
//
//! \brief  Put an element into FIFO without protect.
//!
//! \param  [in]  pFIFO is the pointer of valid FIFO.
//! \param  [in]  pElement is the address of element you want to put
//!
//! \retval 0 if operate successfully, otherwise return -1.
//
//******************************************************************************************
char fifo_put_noprotect(fifo_t *p_fifo, void *p_element)
{
    //! Check input parameters.
    ASSERT(p_fifo);
    ASSERT(p_element);

    // Full ?
    if (0 == p_fifo->free_num)
    {
        //! Error, FIFO is full!
        return (-1);
    }

    if (p_fifo->p_write_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_write_addr = p_fifo->p_start_addr;
    }

    memcpy(p_fifo->p_write_addr, p_element, p_fifo->unit_size);
    p_fifo->p_write_addr += p_fifo->unit_size;
    p_fifo->free_num--;
    p_fifo->used_num++;

    return (0);
}

//******************************************************************************************
//
//! \brief  Get an element from FIFO without protect.
//!
//! \param  [in]  pFIFO is the pointer of valid FIFO.
//! \param  [out] pElement is the address of element you want to get
//!
//! \retval 0 if operate successfully, otherwise return -1.
//
//******************************************************************************************
char fifo_get(fifo_t *p_fifo, void *p_element)
{
    //! Check input parameters.
    ASSERT(p_fifo);
    ASSERT(p_element);

    // Empty ?
    if (0 == p_fifo->used_num)
    {
        //! Error, FIFO is Empty!
        return (-1);
    }

    //! Copy Data
    MUTEX_LOCK(p_fifo->mutex);

    if (0 == p_fifo->used_num)
    {
        //! Error, FIFO is Empty!
        MUTEX_UNLOCK(p_fifo->mutex);
        return (-1);
    }

    if (p_fifo->p_read_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_read_addr = p_fifo->p_start_addr;
    }
    memcpy(p_element, p_fifo->p_read_addr, p_fifo->unit_size);
    p_fifo->p_read_addr += p_fifo->unit_size;
    p_fifo->free_num++;
    p_fifo->used_num--;
    MUTEX_UNLOCK(p_fifo->mutex);

    return (0);
}

//******************************************************************************************
//
//! \brief  Get an element from FIFO.
//!
//! \param  [in]  pFIFO is the pointer of valid FIFO.
//! \param  [out] pElement is the address of element you want to get
//!
//! \retval 0 if operate successfully, otherwise return -1.
//
//******************************************************************************************
char fifo_get_noprotect(fifo_t *p_fifo, void *p_element)
{
    //! Check input parameters.
    ASSERT(p_fifo);
    ASSERT(p_element);

    // Empty ?
    if (0 == p_fifo->used_num)
    {
        //! Error, FIFO is Empty!
        return (-1);
    }

    if (p_fifo->p_read_addr > p_fifo->p_end_addr)
    {
        p_fifo->p_read_addr = p_fifo->p_start_addr;
    }
    memcpy(p_element, p_fifo->p_read_addr, p_fifo->unit_size);
    p_fifo->p_read_addr += p_fifo->unit_size;
    p_fifo->free_num++;
    p_fifo->used_num--;

    return (0);
}

//******************************************************************************************
//
//! \brief  Pre-Read an element from FIFO.
//!
//! \param  [in]  pFIFO is the pointer of valid FIFO.
//! \param  [in]  Offset is the offset from current pointer.
//! \param  [out] pElement is the address of element you want to get
//!
//! \retval 0 if operate successfully, otherwise return -1.
//
//******************************************************************************************
char fifo_pre_read(fifo_t *p_fifo, char offset, void *p_element)
{
    char *_pre_red_index = (void *)0;

    //! Check input parameters.
    ASSERT(p_fifo);
    ASSERT(p_element);

    // OverFlow ?
    if (offset >= p_fifo->used_num)
    {
        return (-1);
    }

    // Move Read Pointer to right position
    _pre_red_index = p_fifo->p_read_addr + p_fifo->unit_size * offset;
    while (_pre_red_index > p_fifo->p_end_addr)
    {
        _pre_red_index = _pre_red_index - p_fifo->p_end_addr + p_fifo->p_start_addr - 1;
    }
    //! Copy Data
    memcpy(p_element, _pre_red_index, p_fifo->unit_size);

    return (0);
}

//******************************************************************************************
//
//! \brief  FIFO is empty ?
//!
//! \param  [in] pFIFO is the pointer of valid FIFO.
//!
//! \retval - None-zero(true) if empty.
//!         - Zero(false) if not empty.
//
//******************************************************************************************
uint8_t fifo_is_empty(fifo_t *p_fifo)
{
    //! Check input parameter.
    ASSERT(p_fifo);

    return (0 == p_fifo->used_num);
}

//******************************************************************************************
//
//! \brief  FIFO is full ?
//!
//! \param  [in] pFIFO is the pointer of valid FIFO.
//!
//! \retval - None-zero(true) if full.
//!         - Zero(false) if not full.
//
//******************************************************************************************
uint8_t fifo_is_full(fifo_t *p_fifo)
{
    //! Check input parameter.
    ASSERT(p_fifo);

    return (0 == p_fifo->free_num);
}

//******************************************************************************************
//
//! \brief  Get FIFO the number of elements?
//!
//! \param  [in] pFIFO is the pointer of valid FIFO.
//!
//! \retval The number of elements in FIFO.
//
//******************************************************************************************
uint16_t fifo_used(fifo_t *p_fifo)
{
    //! Check input parameter.
    ASSERT(p_fifo);

    return (p_fifo->used_num);
}

//******************************************************************************************
//
//! \brief  Get FIFO the number of elements?
//!
//! \param  [in] pFIFO is the pointer of valid FIFO.
//!
//! \retval The number of elements in FIFO.
//
//******************************************************************************************
uint16_t fifo_free(fifo_t *p_fifo)
{
    //! Check input parameter.
    ASSERT(p_fifo);

    return (p_fifo->free_num);
}

//******************************************************************************************
//
//! \brief  Flush the content of FIFO.
//!
//! \param  [in] pFIFO is the pointer of valid FIFO.
//!
//! \retval 0 if success, -1 if failure.
//
//******************************************************************************************
char fifo_flush(fifo_t *p_fifo)
{
    //! Check input parameters.
    ASSERT(p_fifo);

    //! Initialize FIFO Control Block.
    MUTEX_LOCK(p_fifo->mutex);
    p_fifo->free_num = (p_fifo->p_end_addr - p_fifo->p_start_addr) / (p_fifo->unit_size);
    p_fifo->used_num = 0;
    p_fifo->p_read_addr = p_fifo->p_start_addr;
    p_fifo->p_write_addr = p_fifo->p_start_addr;
    MUTEX_UNLOCK(p_fifo->mutex);

    return (0);
}
