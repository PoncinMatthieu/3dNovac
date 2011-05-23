
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        17/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_GL_BUFFERDATA_H_
#define NC_GRAPHICS_CORE_GL_BUFFERDATA_H_

#include <string.h>
#include "GLObject.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Template Interface to manage an OpenGL buffer
            /**
                The openGL buffer is directly allocated in the graphic card.
                By default no memory is keep on local, but it't possible to save the data in local
                <br/> <br/>
                typeBuffer can take : <br/>
                    GL_ARRAY_BUFFER : for a data tab <br/>
                    GL_ELEMENT_ARRAY_BUFFER : for an index tab <br/>

                flags peut prendre pour valeur :
                    GL_STREAM_DRAW : you update your data at each render pass <br/>
                    GL_DYNAMIC_DRAW : you update your data frequently, more than once in one render pass <br/>
                    GL_STATIC_DRAW : your data is not frequently update <br/>
            */
            template <typename T>
            class DataBuffer : public Object
            {
                public:
                    DataBuffer();
                    DataBuffer(GLenum typeBuffer, unsigned int size, unsigned int stride, unsigned int flags, const T *dataTab = NULL, bool conserveData = false);
                    virtual ~DataBuffer();

                    /** Init the buffer */
                    void    Init(GLenum typeBuffer);
                    /** Init the buffer */
                    void    Init(GLenum typeBuffer, unsigned int size, unsigned int stride, unsigned int flags, const T *dataTab, bool conserveData = false);
                    /** Update the buffer */
                    void    UpdateData(unsigned int size, unsigned int stride, unsigned int flags, const T *dataTab, bool conserveData = false);
                    /** Update the buffer */
                    void    UpdateData(const T *dataTab, bool conserveData = false);

                    /** Enable the buffer */
                    virtual inline void         Enable() const              {glBindBuffer(_typeBuffer, _index);}
                    /** Disable the buffer */
                    virtual inline void         Disable() const             {glBindBuffer(_typeBuffer, 0);}
                    /** return the gl index of the buffer */
                    virtual inline unsigned int	GetIndex() const            {return _index;}

                    /** Return the size of the buffer */
                    inline unsigned int         Size() const                {return _size;}
                    /** Return the stride of the data in the buffer */
                    inline unsigned int         Stride() const              {return _stride;}

                    /**
                        Map the buffer in local, and return it's pointer. <br/>
                        The access flag should be GL_READ_ONLY, GL_WRITE_ONLY, or GL_READ_WRITE to specifie the access policy of the buffer
                     */
                    T                           *MapBuffer(GLenum access);
                    /** Unmap the buffer */
                    void                        UnmapBuffer();

                    /** Return the local buffer, if no local has been keeped, the ret value will be null */
                    inline T                    *GetLocalBuffer()           {return _dataTab;}
                    /** Delete the local buffer */
                    inline void                 DeleteLocalBuffer()         {if (_dataTab) delete _dataTab; _dataTab = NULL;}

                protected:
                    /** Release the gl buffer */
                    virtual  void   Release();

                    unsigned int    _index;             ///< the gl buffer index
                    unsigned int    _size;              ///< the size of the buffer
                    unsigned int    _stride;            ///< the stride between two data in the buffer
                    GLenum          _typeBuffer;        ///< the type of the buffer (GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER)
                    unsigned int    _flags;             ///< the flag of the buffer (GL_STREAM_DRAW, GL_DYNAMIC_DRAW, GL_STATIC_DRAW)

                    T               *_dataTab;          ///< the local data tab (never used by default)
            };

            template <typename T>
            DataBuffer<T>::DataBuffer()
                : _index(0), _dataTab(NULL)
            {
            }

            template <typename T>
            DataBuffer<T>::DataBuffer(GLenum typeBuffer, unsigned int size, unsigned int stride, unsigned int flags, const T *dataTab, bool conserveData)
            {
                Init(typeBuffer, size, stride, flags, dataTab, conserveData);
            }

            template <typename T>
            void DataBuffer<T>::Init(GLenum typeBuffer)
            {
                NewRef();
                _typeBuffer = typeBuffer;
                glGenBuffers(1, &_index);   // création du buffer
                LOG_DEBUG << "Data Buffer " << _index << " CREATED" << std::endl;
            }

            template <typename T>
            void DataBuffer<T>::Init(GLenum typeBuffer, unsigned int size, unsigned int stride, unsigned int flags, const T *dataTab, bool conserveData)
            {
                NewRef();
                _typeBuffer = typeBuffer;

                glGenBuffers(1, &_index);   // création du buffer
                Enable();
                UpdateData(size, stride, flags, dataTab, conserveData);
                Disable();
                LOG_DEBUG << "Data Buffer " << _index << " CREATED" << std::endl;
            }

            template<typename T>
            void DataBuffer<T>::UpdateData(unsigned int size, unsigned int stride, unsigned int flags, const T *dataTab, bool conserveData)
            {
                if (_index == 0)
                    throw Utils::Exception("GL::DataBuffer", "Can't update the data if it wasn't init before !");

                _size = size;
                if (stride == 0)
                    _stride = 1;
                else
                    _stride = stride;
                _flags = flags;

                unsigned int completeSize =  _size * _stride;
                glBufferData(_typeBuffer, completeSize * sizeof(T), NULL, _flags); // allocation du buffer
                if (dataTab != NULL)
                    UpdateData(dataTab, conserveData);
            }

            template<typename T>
            void DataBuffer<T>::UpdateData(const T *dataTab, bool conserveData)
            {
                if (_index == 0)
                    throw Utils::Exception("GL::DataBuffer", "Can't update the data if it wasn't init before !");

                unsigned int completeSize =  _size * _stride;
                if (conserveData && dataTab != NULL)
                {
                    _dataTab = new T[completeSize];
                    memcpy(_dataTab, dataTab, completeSize * sizeof(T));
                }
                else
                    _dataTab = NULL;

                glBufferSubData(_typeBuffer, 0, completeSize * sizeof(T), dataTab); // transfert des données a partir du DataTab
            }


            template<typename T>
            DataBuffer<T>::~DataBuffer()
            {
                ReleaseRef();
            }

            template<typename T>
            void DataBuffer<T>::Release()
            {
                if (_dataTab != NULL)
                    delete[] _dataTab;
                _dataTab = NULL;
                glDeleteBuffers(1, &_index); // suppression du buffer
                LOG_DEBUG << "Data Buffer " << _index << " DELETED" << std::endl;
                _index = 0;
            }

            template<typename T>
            T    *DataBuffer<T>::MapBuffer(GLenum access)
            {
                T *p = (T*)glMapBuffer(_typeBuffer, access);
                if (p == NULL)
                    LOG_ERROR << "DataBuffer::glMapBuffer: " << EXT.GetError() << std::endl;
                return p;
            }

            template<typename T>
            void   DataBuffer<T>::UnmapBuffer()
            {
                if (glUnmapBuffer(_typeBuffer) == GL_FALSE)
                    LOG_ERROR << "DataBuffer::glUnmapBuffer: " << EXT.GetError() << std::endl;
            }

        }
    }
}

#endif
