/**
 * @file /message_mux_demux/include/message_mux_demux/subscriber.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef message_mux_demux_SUBSCRIBER_HPP_
#define message_mux_demux_SUBSCRIBER_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/utilities/function_objects.hpp>
#include <sstream>
#include <string>
#include "demux.hpp"
#include "exceptions.hpp"
#include "header.hpp"
#include "messages.hpp"
#include "messages/core.hpp"
#include "registry.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace message_mux_demux {

/*****************************************************************************
** Interfaces
*****************************************************************************/

template<typename DataType, unsigned int ID>
class Subscriber {
public:
  /**
   * Construct with a callback to a global or static function.
   *
   * @param name : name of the demux to connect to
   * @param id : command id of the packet to associate with
   * @param f : the global/static function.
   */
  Subscriber(const std::string& name,
             void (*f)(const DataType&)) :
    name(name),
    id(ID),
    function(new ecl::UnaryFreeFunction<const DataType&>(f))
  {
    if ( !MessageRegistry::isRegistered(ID) ) {
      std::stringstream ss;
      ss << "id '" << id << "' has not been registered";
      throw UnregisteredID(ss.str());
    }
    if ( !MessageRegistry::isRegisteredWithType<DataType>(ID) ) {
      std::stringstream ss;
      ss << "id '" << id << "' is registered, but not with this type";
      throw InvalidIDTypeCombination(ss.str());
    }
    MessageDemux::registerSubscriber(name, id, &Subscriber<DataType, ID>::processPacket, (*this));
  }
  /**
   * Construct with a callback to a member function.
   *
   * @param name : name of the demux to connect to
   * @param id : command id of the packet to associate with
   * @param f : the member function to load.
   * @param c : the instance for the member function's class.
   * @tparam C : the member function's class type.
   */
  template<typename C>
  Subscriber(const std::string& name,
             void (C::*f)(const DataType&), C &c) :
    name(name),
    id(ID),
    function(new ecl::PartiallyBoundUnaryMemberFunction<C,const DataType&,void>(f,c))
  {
    MessageDemux::registerSubscriber(name, id, &Subscriber<DataType, ID>::processPacket, (*this));
  }
  virtual ~Subscriber() {
    MessageDemux::unregisterSubscriber(name, id);
    delete function;
  }

  void processPacket(const unsigned char* buffer, const unsigned int& size) {
    DataType data = message_mux_demux::Message<DataType>::decode(buffer, size);
    (*function)(data);
  }

private:
  std::string name;
  unsigned int id;
  ecl::UnaryFunction<const DataType&,void> *function;
};

} // namespace message_mux_demux

#endif /* message_mux_demux_SUBSCRIBER_HPP_ */
