#pragma once

#include <sstream>
#include <iostream>

#include "Foundation/Memory/HybridPtr.h"
#include "Foundation/Automation/Event.h"
#include "Foundation/SmartBuffer/BasicBuffer.h"

#include "Element.h"
#include "Archive.h"

#include "Foundation/Atomic.h"

namespace Helium
{
    namespace Reflect
    {
        namespace SerializerFlags
        {
            enum SerializerFlag
            {
                Shallow   = 1 << 0,
            };
        }

        typedef SerializerFlags::SerializerFlag SerializerFlag;

        struct TranslateEventArgs
        {
            // the serialier to read/write from
            Serializer* m_Serializer;

            TranslateEventArgs( Serializer* serializer )
                : m_Serializer (serializer)
            {

            }
        };

        struct TranslateInputEventArgs : TranslateEventArgs
        {
            // the stream to read from
            tistream& m_Stream;

            TranslateInputEventArgs( Serializer* serializer, tistream& stream )
                : TranslateEventArgs (serializer)
                , m_Stream (stream)
            {

            }
        };
        typedef Helium::Signature< void, TranslateInputEventArgs&, Helium::AtomicRefCountBase > TranslateInputSignature;

        struct TranslateOutputEventArgs : TranslateEventArgs
        {
            // the stream to write to
            tostream& m_Stream;

            TranslateOutputEventArgs( Serializer* serializer, tostream& stream )
                : TranslateEventArgs (serializer)
                , m_Stream (stream)
            {

            }
        };
        typedef Helium::Signature< void, TranslateOutputEventArgs&, Helium::AtomicRefCountBase > TranslateOutputSignature;


        //
        // A Serializer is an Element that knows how to read/write data
        //  from any kind of support Archive type (XML and Binary), given
        //  an address in memory to serialize/deserialize data to/from
        //

        class FOUNDATION_API Serializer : public Element
        {
        protected:
            // derived classes will use this
            template<class T>
            class DataPtr
            {
            private:
                // owned or borrowed data
                Helium::HybridPtr<T> m_Target;

                // owned data
                T m_Primitive;

            public:
                DataPtr()
                {
                    m_Target = &m_Primitive;
                }

                void Connect(Helium::HybridPtr<T> pointer)
                {
                    if (pointer.Address())
                    {
                        m_Target = pointer;
                    }
                    else
                    {
                        m_Target = &m_Primitive;
                    }
                }

                void Disconnect()
                {
                    m_Target = &m_Primitive;
                }

                void Set(const T& val)
                {
                    *m_Target = val;
                }

                const T& Get() const
                {
                    return *m_Target;
                }

                const T* operator-> () const
                {
                    return m_Target;
                }

                T* operator-> ()
                {
                    return m_Target;
                }

                const T* Ptr() const
                {
                    return m_Target;
                }

                T* Ptr()
                {
                    return m_Target;
                }

                const T& Ref() const
                {
                    return *m_Target;
                }

                T& Ref()
                {
                    return *m_Target;
                }

                T Val() const
                {
                    return *m_Target;
                }
            };

            // the instance we are processing, if any
            Helium::HybridPtr<Element> m_Instance;

            // the field we are processing, if any
            const Field* m_Field;

        public:
            REFLECT_DECLARE_ABSTRACT( Serializer, Element );

            // instance init and cleanup
            Serializer();
            virtual ~Serializer();

            // static init and cleanup
            static void Initialize();
            static void Cleanup();


            //
            // Connection
            //

            // connect to some address
            virtual void ConnectData(Helium::HybridPtr<void> data)
            {
                m_Instance = (Element*)NULL;
                m_Field = NULL;
            }

            // connect to a field of an object
            virtual void ConnectField(Helium::HybridPtr<Element> instance, const Field* field, uintptr offsetInField = 0)
            {
                ConnectData( Helium::HybridPtr<void>( instance.Address() + field->m_Offset + offsetInField, instance.State())); 

                m_Instance = instance; 
                m_Field = field; 
            }

            // disconnect everything
            void Disconnect()
            {
                ConnectData( Helium::HybridPtr<void> () );
            }


            //
            // Deduction templates (params are concrete types)
            //

            template<class T>
            static inline T* GetData(Serializer*)
            {
                return NULL;
            }

            template<class T>
            static inline const T* GetData(const Serializer*)
            {
                return NULL;
            }


            //
            // Creation templates
            //

            template <class T>
            static SerializerPtr Create()
            {
                i32 type = Reflect::GetType<T>();

                HELIUM_ASSERT( type != Reflect::ReservedTypes::Invalid );

                return AssertCast<Serializer>( Registry::GetInstance()->CreateInstance(type) );
            }

            template <class T>
            static SerializerPtr Create(const T& value)
            {
                SerializerPtr ser = Create<T>();

                if (ser.ReferencesObject())
                {
                    SetValue(ser, value);
                }

                return ser;
            }

            template <class T>
            static SerializerPtr Bind(T& value, Element* instance, const Field* field)
            {
                SerializerPtr ser = Create<T>();

                if (ser.ReferencesObject())
                {
                    ser->ConnectData( &value );
                    ser->m_Instance = instance;
                    ser->m_Field = field;
                }

                return ser;
            }

            template <class T>
            static SerializerPtr Bind(const T& value, const Element* instance, const Field* field)
            {
                SerializerPtr ser = Create<T>();

                if (ser.ReferencesObject())
                {
                    ser->ConnectData( &value );
                    ser->m_Instance = instance;
                    ser->m_Field = field;
                }

                return ser;
            }


            //
            // Value templates
            //

            template <typename T>
            static bool GetValue(const Serializer* ser, T& value);

            template <typename T>
            static bool SetValue(Serializer* ser, const T& value, bool raiseEvents = true);


            //
            // Data Management
            //

            // check to see if a cast is supported
            static bool CastSupported(i32 srcType, i32 destType);

            // convert value data from one serializer to another
            static bool CastValue(const Serializer* src, Serializer* dest, u32 flags = 0);

            // copies value data from one serializer to another
            virtual bool Set(const Serializer* src, u32 flags = 0) = 0;

            // assign
            Serializer& operator=(const Serializer* rhs)
            {
                Set(rhs);
                return *this;
            }
            Serializer& operator=(const Serializer& rhs)
            {
                Set(&rhs);
                return *this;
            }

            // equality of connected data
            virtual bool Equals(const Serializer* src) const = 0;

            // equality
            bool operator==(const Serializer* rhs) const
            {
                return Equals(rhs);
            }
            bool operator== (const Serializer& rhs) const
            {
                return Equals(&rhs);
            }

            // inequality
            bool operator!= (const Serializer* rhs) const
            {
                return !Equals(rhs);
            }
            bool operator!= (const Serializer& rhs) const
            {
                return !Equals(&rhs);
            }


            //
            // Serialization
            //

            // data serialization (extract to smart buffer)
            virtual void Serialize (const Helium::BasicBufferPtr& buffer, const tchar* debugStr) const
            {
                HELIUM_BREAK();
            }

            // data serialization (extract to archive)
            virtual void Serialize(Archive& archive) const = 0;

            // data deserialization (insert from archive)
            virtual void Deserialize(Archive& archive) = 0;

            // text serialization (extract to text stream)
            virtual tostream& operator>> (tostream& stream) const
            { 
                HELIUM_BREAK(); 
                return stream; 
            }

            // text deserialization (insert from text stream)
            virtual tistream& operator<< (tistream& stream)
            { 
                HELIUM_BREAK(); 
                return stream; 
            }


            //
            // Visit
            //

            virtual void Host (Visitor& visitor) HELIUM_OVERRIDE
            {
                // by default, don't do anything as it will all have to be special cased in derived classes
            }


            //
            // Translate
            //

        protected:
            TranslateOutputSignature::Delegate m_TranslateOutput;
            TranslateInputSignature::Delegate m_TranslateInput;

        public:
            void SetTranslateInputListener( const TranslateInputSignature::Delegate& listener )
            {
                m_TranslateInput = listener;
            }
            void SetTranslateOutputListener( const TranslateOutputSignature::Delegate& listener )
            {
                m_TranslateOutput = listener;
            }

        protected:
            bool TranslateOutput( tostream& stream ) const;
            bool TranslateInput( tistream& stream );
        };


        //
        // These are the global entry points for our virtual insert an extract functions
        //

        inline tostream& operator<< (tostream& stream, const Serializer& s)
        {
            return s >> stream;
        }

        inline tistream& operator>> (tistream& stream, Serializer& s)
        {
            return s << stream;
        }


        //
        // GetValue / SetValue templates
        //

        template <typename T>
        inline bool Serializer::GetValue(const Serializer* ser, T& value)
        {
            if ( ser == NULL )
            {
                return false;
            }

            bool result = false;
            int type = Reflect::GetType<T>();

            // if you die here, then you are not using serializers that
            //  fully implement the type deduction functions above
            HELIUM_ASSERT( type != Reflect::ReservedTypes::Invalid );

            // sanity check our element type
            if ( ser->HasType(type) )
            {
                // get internal data pointer
                const T* data = GetData<T>( ser );
                HELIUM_ASSERT( data != NULL );

                // make the copy
                value = *data;
                result = true;
            }
            else
            {
                // create a temporary serializer of the value type
                SerializerPtr temp = AssertCast<Serializer>( Registry::GetInstance()->CreateInstance( type ) );

                // connect the temp serializer to the temp value
                T tempValue; temp->ConnectData( &tempValue );

                // cast into the temp value
                if (Serializer::CastValue( ser, temp ))
                {
                    // make the copy
                    value = tempValue;
                    result = true;
                }
            }

            HELIUM_ASSERT(result);
            return result;
        }

        template <typename T>
        inline bool Serializer::SetValue(Serializer* ser, const T& value, bool raiseEvents)
        {
            if ( ser == NULL )
            {
                return false;
            }

            bool result = false;
            int type = Reflect::GetType<T>();

            // if you die here, then you are not using serializers that
            //  fully implement the type deduction functions above
            HELIUM_ASSERT( type != Reflect::ReservedTypes::Invalid );

            // sanity check our element type
            if ( ser->HasType(type) )
            {
                // get internal data pointer
                T* data = GetData<T>( ser );
                HELIUM_ASSERT( data != NULL );

                // if you die here, then you are probably in release and this should crash
                (*data) = value;
                result = true;
            }
            else
            {
                // create a temporary serializer of the value type
                SerializerPtr temp = AssertCast<Serializer>( Registry::GetInstance()->CreateInstance( type ) );

                // connect the temp serializer to the temp value
                temp->ConnectData( &value );

                // cast into the serializer
                if (Serializer::CastValue( ser, temp ))
                {
                    result = true;
                }
            }

            if (result)
            {
                // Notify interested listeners that the data has changed.
                if ( raiseEvents && ser && ser->m_Instance && ser->m_Field && ser->m_Field->m_Type->GetReflectionType() == ReflectionTypes::Class )
                {
                    Element* element = (Element*)( ser->m_Instance );
                    element->RaiseChanged( ser->m_Field );
                }
            }

            HELIUM_ASSERT(result);
            return result;
        }
    }
}

#define REFLECT_SPECIALIZE_SERIALIZER(Name) \
typedef Helium::SmartPtr< Name > ##Name##Ptr; \
template<> static inline i32 Reflect::GetType<Name::DataType>() \
{ \
    return Reflect::GetType<Name>(); \
} \
template<> static inline Name::DataType* Serializer::GetData<Name::DataType>( Serializer* serializer ) \
{ \
    return serializer && serializer->GetType() == Reflect::GetType<Name::DataType>() ? static_cast<Name*>( serializer )->m_Data.Ptr() : NULL; \
} \
template<> static inline const Name::DataType* Serializer::GetData<Name::DataType>( const Serializer* serializer ) \
{ \
    return serializer && serializer->GetType() == Reflect::GetType<Name::DataType>() ? static_cast<const Name*>( serializer )->m_Data.Ptr() : NULL; \
}
