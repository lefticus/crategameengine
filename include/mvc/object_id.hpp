#ifndef __mvc_object_id__
#define __mvc_object_id__

#include <string>
#include <stdexcept>

namespace mvc
{
  struct object_id_base
  {
    protected:
      object_id_base(const std::string &t_class_name, 
          const std::string &t_id)
        : class_name(t_class_name), id(t_id)
      {
      }


    public:
      object_id_base(const object_id_base &oid)
        : class_name(oid.class_name),
          id(oid.id)
      {
      }

      object_id_base()
        : class_name(""),
          id("")
      {
      }


      struct invalid_object_type : std::runtime_error
      {
        invalid_object_type()
          : std::runtime_error("object_id types are incompatible")
        {
        }
      };

      bool operator<(const object_id_base &oid) const
      {
        return class_name < oid.class_name || (class_name == oid.class_name && id < oid.id);
      }

      std::string class_name;
      std::string id;
  };

  template<typename T>
  struct object_id : object_id_base
  {
    typedef T object_type;

    object_id(const std::string &t_id)
      : object_id_base(T::class_name, t_id)
    {
    }

    object_id(const object_type &t_obj)
      : object_id_base(t_obj.oid)
    {
    }

    object_id()
      : object_id_base(T::class_name, "")
    {
    }

    explicit object_id(const object_id_base &oid)
      : object_id_base(oid)
    {
      if (oid.class_name != object_type::class_name) throw invalid_object_type();
    }
  };
}

#endif
