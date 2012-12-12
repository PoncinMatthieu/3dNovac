
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

#ifndef NC_GRAPHICS_GL_BUFFERDATA_H_
#define NC_GRAPHICS_GL_BUFFERDATA_H_

#include <string.h>
#include "GLObject.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Template Interface to manage an OpenGL buffer.
            /**
                The openGL buffer is directly allocated in the graphic card.
                The memory is not keep in local, but it't possible to retreive it by using the MapBuffer method.
            */
            template <typename T>
            class DataBuffer : public Object
            {
                public:
                    DataBuffer();
                    DataBuffer(Enum::DataBuffer::Target target, unsigned int size, unsigned int stride, Enum::DataBuffer::Usage usage, const T *dataTab = NULL);
                    virtual ~DataBuffer();

                    /** Create a new copy of the object by without duplicate the ogl resources. */
                    virtual Object          *Clone() const                  {return new DataBuffer<T>(*this);}

                    /** Init the buffer. */
                    void    Init(Enum::DataBuffer::Target target);
                    /** Init the buffer. */
                    void    Init(Enum::DataBuffer::Target target, unsigned int size, unsigned int stride, Enum::DataBuffer::Usage usage, const T *dataTab);
                    /** Update the buffer. */
                    void    UpdateData(unsigned int size, unsigned int stride, Enum::DataBuffer::Usage usage, const T *dataTab);
                    /** Update the buffer. */
                    void    UpdateData(const T *dataTab);

                    /** Enable the buffer. */
                    virtual void            Enable() const;
                    // /** Disable the buffer */
                    //virtual inline void         Disable() const             {State::Current().Unbind(_target);} // since OpenGL version 3.3, unbind a buffer is unnecessary
                    /** return the gl index of the buffer. */
                    virtual unsigned int	GetIndex() const                {return _index;}

                    /** Return the size of the buffer. */
                    inline unsigned int         Size() const                {return _size;}
                    /** Return the stride of the data in the buffer. */
                    inline unsigned int         Stride() const              {return _stride;}

                    /**
                        Map the buffer in the local buffer, and return it's pointer.
                     */
                    T                           *MapBuffer(Enum::DataBuffer::AccessType access);
                    /** Unmap the buffer. */
                    void                        UnmapBuffer();

                    /** \return the local buffer, if no local has been keeped, the ret value will be null. */
                    inline T                    *LocalBuffer()              {return _dataTab;}

                protected:
                    /** Release the gl buffer. */
                    virtual  void   Release();

                    unsigned int                _index;             ///< the gl buffer index.
                    unsigned int                _size;              ///< the size of the buffer.
                    unsigned int                _stride;            ///< the stride between two data in the buffer.
                    Enum::DataBuffer::Target    _target;            ///< the target to which the buffer object is bound.
                    Enum::DataBuffer::Usage     _usage;             ///< the type of usage of the buffer.

                    T                           *_dataTab;          ///< the local data tab (never used by default).
            };

            template <typename T>
            DataBuffer<T>::DataBuffer()
                : _index(0), _size(0), _stride(1), _target(Enum::DataBuffer::ArrayBuffer), _usage(Enum::DataBuffer::StaticDraw), _dataTab(NULL)
            {
            }

            template <typename T>
            DataBuffer<T>::DataBuffer(Enum::DataBuffer::Target target, unsigned int size, unsigned int stride, Enum::DataBuffer::Usage usage, const T *dataTab)
            {
                Init(target, size, stride, usage, dataTab);
            }

            template <typename T>
            void    DataBuffer<T>::Enable() const
            {
                if (State::IsSet())
                    State::Current().Bind(_target, _index);
                else
                    glBindBuffer(_target, _index);
            }


            template <typename T>
            void    DataBuffer<T>::Init(Enum::DataBuffer::Target target)
            {
                CALLSTACK_INFO("DataBuffer::Init() ");
                NewRef();
                _target = target;
                glGenBuffers(1, &_index);   // création du buffer
                CALLSTACK_APPEND_INFO(Utils::Convert::ToString(_index));
                NC_GRAPHIC_GL_CHECK_ERROR();
            }

            template <typename T>
            void DataBuffer<T>::Init(Enum::DataBuffer::Target target, unsigned int size, unsigned int stride, Enum::DataBuffer::Usage usage, const T *dataTab)
            {
                CALLSTACK_INFO("DataBuffer::Init() ");
                NewRef();
                _target = target;

                glGenBuffers(1, &_index);   // création du buffer
                CALLSTACK_APPEND_INFO(Utils::Convert::ToString(_index));
                NC_GRAPHIC_GL_CHECK_ERROR();

                Enable();
                UpdateData(size, stride, usage, dataTab);
                Disable();
            }

            template<typename T>
            void DataBuffer<T>::UpdateData(unsigned int size, unsigned int stride, Enum::DataBuffer::Usage usage, const T *dataTab)
            {
                CALLSTACK_INFO("DataBuffer::UpdateData(" + Utils::Convert::ToString(size) + ", " + Utils::Convert::ToString(stride) + ")");
                if (_index == 0)
                    throw Utils::Exception("GL::DataBuffer", "Can't update the data if it wasn't init before !");

                _size = size;
                if (stride == 0)
                    _stride = 1;
                else
                    _stride = stride;
                _usage = usage;

                unsigned int completeSize =  _size * _stride;
                glBufferData(_target, completeSize * sizeof(T), NULL, _usage); // allocation du buffer
                NC_GRAPHIC_GL_CHECK_ERROR();
                if (dataTab != NULL)
                    UpdateData(dataTab);
            }

            template<typename T>
            void DataBuffer<T>::UpdateData(const T *dataTab)
            {
                CALLSTACK_INFO("DataBuffer::UpdateData()");
                if (_index == 0)
                    throw Utils::Exception("GL::DataBuffer", "Can't update the data if it wasn't init before !");

                unsigned int completeSize =  _size * _stride;
                _dataTab = NULL;
                glBufferSubData(_target, 0, completeSize * sizeof(T), dataTab); // transfert des données a partir du DataTab
                NC_GRAPHIC_GL_CHECK_ERROR();
            }


            template<typename T>
            DataBuffer<T>::~DataBuffer()
            {
                ReleaseRef();
            }

            template<typename T>
            void DataBuffer<T>::Release()
            {
                CALLSTACK_INFO("DataBuffer::Release() " + Utils::Convert::ToString(_index));
                _dataTab = NULL;
                glDeleteBuffers(1, &_index); // suppression du buffer
                _index = 0;
                NC_GRAPHIC_GL_CHECK_ERROR();
            }

            template<typename T>
            T    *DataBuffer<T>::MapBuffer(Enum::DataBuffer::AccessType access)
            {
                CALLSTACK_INFO("DataBuffer::MapBuffer()");
                _dataTab = (T*)glMapBuffer(_target, access);
                if (_dataTab == NULL)
                    LOG_ERROR << "DataBuffer glMapBuffer" << State::GetError() << std::endl;
                return _dataTab;
            }

            template<typename T>
            void   DataBuffer<T>::UnmapBuffer()
            {
                CALLSTACK_INFO("DataBuffer::UnmapBuffer()");
                _dataTab = NULL;
                if (glUnmapBuffer(_target) == GL_FALSE)
                    LOG_ERROR << "DataBuffer::glUnmapBuffer: " << State::GetError() << std::endl;
            }

        }
    }
}

#endif
