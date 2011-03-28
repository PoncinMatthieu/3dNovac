
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
/*-----------------------------------------------------------------------------

                    Implementation de la classe "gDataBuffer"

                  Classe permettant de definir un buffer OpenGL
    (Buffer directement aloué dans la memoire graphique, permettant un
     gain considerable en performance et de limiter la bande passante
                            entre le CPU et le GPU)

          !! Le buffer ne doit contenir qu'un seul type de données

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
            template <typename T>
            class DataBuffer : public Object
            {
                public:
                /*
                aTypeBuffer peut prendre pour valeur :  GL_ARRAY_BUFFER : pour un tableau de données
                                                        GL_ELEMENT_ARRAY_BUFFER : pour un tableau d'indices

                aFlags peut prendre pour valeur :   GL_STREAM_DRAW : vous mettez vos données à jour à chaque affichage ( glDraw*() )
                                                    GL_DYNAMIC_DRAW : vous mettez fréquemment à jour vos données. (plus d'une fois par affichage)
                                                    GL_STATIC_DRAW : vous mettez peu souvent voir jamais vos données à jour
                */
                    DataBuffer();
                    DataBuffer(GLenum typeBuffer, unsigned int size, unsigned int stride, unsigned int flags, const T *dataTab = NULL, bool conserveData = false);
                    virtual ~DataBuffer();

                    void    Init(GLenum typeBuffer);
                    void    Init(GLenum typeBuffer, unsigned int size, unsigned int stride, unsigned int flags, const T *dataTab, bool conserveData = false);
                    void    UpdateData(unsigned int size, unsigned int stride, unsigned int flags, const T *dataTab, bool conserveData = false);
                    void    UpdateData(const T *dataTab, bool conserveData = false);

                    virtual inline void         Enable() const              {glBindBuffer(_typeBuffer, _index);}
                    virtual inline void         Disable() const             {glBindBuffer(_typeBuffer, 0);}
                    virtual inline unsigned int	GetIndex() const            {return _index;}

                    inline unsigned int         Size() const                {return _size;}
                    inline unsigned int         Stride() const              {return _stride;}

                    inline T                    *MapBuffer(GLenum access)   {return (T*)glMapBuffer(_typeBuffer, access);}
                    inline void                 UnmapBuffer()               {glUnmapBuffer(_typeBuffer);}

                    inline T                    *GetLocalBuffer()            {return _dataTab;}
                    inline void                 DeleteLocalBuffer()         {if (_dataTab) delete _dataTab; _dataTab = NULL;}

                protected:
                    virtual  void   Release();

                    unsigned int    _index;
                    unsigned int    _size;
                    unsigned int    _stride;
                    GLenum          _typeBuffer;
                    unsigned int    _flags;
                    bool            _conserveData;

                    T               *_dataTab;
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
                if (_conserveData && dataTab != NULL)
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
        }
    }
}

#endif
