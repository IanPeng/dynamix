// DynaMix
// Copyright (c) 2013-2016 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once


/**
 * \file
 * Macros used to declare and define messages.
 */


#include "message.hpp"
#include "preprocessor.hpp"
#include "assert.hpp"

// some macros here have an underscore in front so it doesn't appear as a suggestion in
// ides that support these
// that shows that they're for internal use only

/// \internal
#define _DYNAMIX_MESSAGE_STRUCT_NAME(message_name) _DYNAMIX_PP_CAT(dynamix_msg_, message_name)
/// \internal
#define _DYNAMIX_MESSAGE_TAG(message_name) _DYNAMIX_PP_CAT(message_name, _msg)

// a macro used in messages to get the mixin data directly, skipping function calls
// GREATLY improves message call time
/// \internal
#define _DYNAMIX_GET_MIXIN_DATA(obj, id) \
    reinterpret_cast<char*>(const_cast<void*>(obj._mixin_data[obj._type_info->_mixin_indices[id]].mixin()))


// default impl helper macros

// name of default implementation struct
/// \internal
#define DYNAMIX_DEFAULT_IMPL_STRUCT(message_name) _DYNAMIX_PP_CAT(message_name, _default_impl_t)

#if defined(DYNAMIX_DOXYGEN)
// use these macros for the docs only

/**
* Macro that declares a message
*
* This generates the stand-alone function which is users should use to call messages.
*
* The calls will throw the exception `bad_message_call` if none of the mixins
* from which the object is composed handles this message. To prevent the message calls
* from Throwing exceptions you can define `DYNAMIX_NO_MSG_THROW` before including
* the library's headers.
*
* \par Variants:
* \code
* DYNAMIX_MESSAGE_N(return_type, message, args)
* DYNAMIX_CONST_MESSAGE_N(return_type, message, args)
* DYNAMIX_MULTICAST_MESSAGE_N(return_type, message, args)
* DYNAMIX_CONST_MULTICAST_MESSAGE_N(return_type, message, args)
* DYNAMIX_EXPORTED_MESSAGE_N(export, return_type, message, args)
* DYNAMIX_EXPORTED_CONST_MESSAGE_N(export, return_type, message, args)
* DYNAMIX_EXPORTED_MULTICAST_MESSAGE_N(export, return_type, message, args)
* DYNAMIX_EXPORTED_CONST_MULTICAST_MESSAGE_N(export, return_type, message, args)
* DYNAMIX_MESSAGE_N_OVERLOAD(message_name, return_type, method_name, args)
* DYNAMIX_CONST_MESSAGE_N_OVERLOAD(message_name, return_type, method_name, args)
* DYNAMIX_MULTICAST_MESSAGE_N_OVERLOAD(message_name, return_type, method_name, args)
* DYNAMIX_CONST_MULTICAST_MESSAGE_N_OVERLOAD(message_name, return_type, method_name, args)
* DYNAMIX_EXPORTED_MESSAGE_N_OVERLOAD(export, message_name, return_type, method_name, args)
* DYNAMIX_EXPORTED_CONST_MESSAGE_N_OVERLOAD(export, message_name, return_type, method_name, args)
* DYNAMIX_EXPORTED_MULTICAST_MESSAGE_N_OVERLOAD(export, message_name, return_type, method_name, args)
* DYNAMIX_EXPORTED_CONST_MULTICAST_MESSAGE_N_OVERLOAD(export, message_name, return_type, method_name, args)
* \endcode
*
* \par Legend:
* - `N` in those variant names is a number that indicates how many parameters the
* message takes. If `N` is 0, then `args` is omitted.
* - `args` is a coma-separated list of the argument types and argument names of
* the message's arguments
* - If `MULTICAST` is a part of a macro, it declares a multicast message. Otherwise
* it declares a unicast message.
* - If `CONST` is part of a macro, then the message works with `const object&` and
* should be bound to const methods of a mixin class. Otherwise it works with simply
* `object&` and should be bound no non-const methods of the mixin class.
* - If `EXPORTED` is part of a macro, then it's used to declare a message that is
* exported from a dynamic library. The `export` parameter should be the appropriate
* export/import symbols for the particular compiler (i.e. `__declspec(dllexport)`)
* - If `OVERLOAD` is part of a macro, it defines a message overload. It splits the
* ways of referring to the message in two. The first - `message_name` - should be
* used when referring to the message - in mixin feature lists, in
* `object::implements`, and in `DYNAMIX_DEFINE_MESSAGE`. The second -
* `method_name` - is the name of the method in the mixin class, and will be the
* name of the message function generated by the macro.
*
* \par Examples:
* \code
*
* // A basic non-const unicast message with no arguments.
* // Should be bound to: void mixin_class::foo()
* DYNAMIX_MESSAGE_0(void, foo);
*
* // A const multicast message with two arguments
* // Should be bound to: void mixin_class::serialize(archive& ar, int flags) const
* DYNAMIX_CONST_MULTICAST_MESSAGE_2(void, serialize, archive&, ar, int, flags);
*
* // Assuming MY_LIB_API is a macro that expands accordingly to the
* // export/import symbols for the compiler you're using, this is
* // a const unicast message with one argument exported from a dynamic library
* // Should be bound to: float mixin_class::size(int dimension) const
* DYNAMIX_EXPORTED_CONST_MESSAGE_1(MY_LIB_API, int, size, int, dimension);
*
* // Two message overloads, that should be bound to:
* // void mixin_class::scale(float uniform);
* // void mixin_class::scale(const vector3& vec);
* // respectively.
* // The macros will generate two functions named scale with the
* // appropriate arguments. In order to bind them to a mixin,
* // you should use scale_uniform_msg and scale_vector_msg
* // in the mixin feature list.
* DYNAMIX_MESSAGE_1_OVERLOAD(scale_uniform, void, scale, float, uniform);
* DYNAMIX_MESSAGE_1_OVERLOAD(scale_vector, void, scale, const vector3&, vec);
* \endcode
*/
#define DYNAMIX_MESSAGE_N(return_type, message, args)

/**
* Macro that defines a message by also providing a default implementation
*
* Use it once per message in a compilation unit (.cpp file)
*
* A default implementation lets the user provide code to be executed if
* a message is called for an object which has no mixin implementing it.
*
* \par Legend:
* - `N` is a number that indicates how many parameters the
* message takes. If `N` is 0, then `args` is omitted.
* - `args` is a coma-separated list of the argument types and argument names of
* the message's arguments
* - `message_name` - is the name of the message
*
* \par Notes:
* - As is the case with the mutation rules, an empty object won't implement default
* message implementations. You'll need at least one mutation to make it do so
* - A default implementation can be treated the same way as your implementation
* in the method for a regular message: ie `dm_this` is valid
* - If any of the mixins in an object implements this message, the default
* implementation will be unreachable. Multicasts won't feature it.
*/
#define DYNAMIX_DEFINE_MESSAGE_N_WITH_DEFAULT_IMPL(return_type, message_name, args)
#else
// include the generated macros
#include "gen/message_macros.hpp"
#endif

/**
 * The macro for defining a message.
 * Use it once per message in a compilation unit (.cpp file)
 */
#define DYNAMIX_DEFINE_MESSAGE(message_name) \
    /* create a feature getter for the message */ \
    ::dynamix::feature& _dynamix_get_mixin_feature(const _DYNAMIX_MESSAGE_STRUCT_NAME(message_name)*) \
    { \
        return ::dynamix::internal::feature_instance<_DYNAMIX_MESSAGE_STRUCT_NAME(message_name)>::the_feature(); \
    } \
    /* create a feature registrator */ \
    void _dynamix_register_mixin_feature(const _DYNAMIX_MESSAGE_STRUCT_NAME(message_name)*) \
    { \
        ::dynamix::internal::domain::instance(). \
            register_feature(::dynamix::internal::feature_instance<_DYNAMIX_MESSAGE_STRUCT_NAME(message_name)>::the_feature()); \
    } \
    /* provide a tag instance */ \
    _DYNAMIX_MESSAGE_STRUCT_NAME(message_name) * _DYNAMIX_MESSAGE_TAG(message_name)
