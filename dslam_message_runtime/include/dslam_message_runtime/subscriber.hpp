/**
 * @file /dslam_message_runtime/include/dslam_message_runtime/subscriber.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef dslam_message_runtime_SUBSCRIBER_HPP_
#define dslam_message_runtime_SUBSCRIBER_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/utilities/function_objects.hpp>
#include <string>
#include "demux.hpp"
#include "message.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

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
    std::cout << "Set up unary function foo"<< std::endl;
    (*function)(std::string("foo"));
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
    std::cout << "Set up partially bound member function foo"<< std::endl;
    MessageDemux::registerSubscriber(name, id, &Subscriber<DataType, ID>::processPacket, (*this));
  }
  virtual ~Subscriber() {
    MessageDemux::unregisterSubscriber(name, id);
    delete function;
  }

  void processPacket(const char* buffer) {
    std::cout << "Processing Packet" << std::endl;
    DataType data = dslam::Message<DataType>::decode(buffer);
    std::cout << "Sending out: " << data << std::endl;
    (*function)(data);
  }

private:
  std::string name;
  unsigned int id;
  ecl::UnaryFunction<const DataType&,void> *function;
};

} // dslam

#endif /* dslam_message_runtime_SUBSCRIBER_HPP_ */
