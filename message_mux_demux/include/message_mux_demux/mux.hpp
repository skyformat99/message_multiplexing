/**
 * @file /message_mux_demux/include/message_mux_demux/mux.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef message_mux_demux_MUX_HPP_
#define message_mux_demux_MUX_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <map>
#include <memory>
#include <string>
#include "messages.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace message_mux_demux {
namespace impl {

/*****************************************************************************
 ** Interfaces
 *****************************************************************************/

class MessageMux {
public:
  MessageMux(const std::string& name,
             const std::string& url,
             const bool verbose = false);
  ~MessageMux();
  int send(const unsigned int& id, const ByteArray& msg_buffer);

private:
  std::string name, url;
  int socket;
  bool verbose;
};

} // namespace impl
} // message_mux_demux

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace message_mux_demux {

/*****************************************************************************
** Interfaces
*****************************************************************************/

class MessageMux {
public:
  typedef std::map<std::string, std::shared_ptr<impl::MessageMux>> MuxMap;
  typedef std::pair<std::string, std::shared_ptr<impl::MessageMux>> MuxMapPair;
  typedef std::map<std::string, std::shared_ptr<impl::MessageMux>>::iterator MuxMapIterator;
  typedef std::map<std::string, std::shared_ptr<impl::MessageMux>>::const_iterator MuxMapConstIterator;

  enum MessageMuxErrors {
    SpecifiedMuxNotAvailable = -1  // when you try to connect to <name>, but <name> has not yet been started
  };
  static void registerMux(const std::string& name, const std::string& url, const bool verbose=false);
  static void unregisterMux(const std::string& name);
  static MuxMap& multiplexers();
  static int send(const std::string& name, const unsigned int& id, const ByteArray& msg_buffer);
//  static int createPublisher(const std::string& name, const std::string& url = "");
};

} // namespace message_mux_demux

#endif /* message_mux_demux_MUX_HPP_ */
