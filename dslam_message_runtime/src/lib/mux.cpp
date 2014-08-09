/**
 * @file /dslam_message_runtime/src/lib/mux.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include "../../include/dslam_message_runtime/mux.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Implementation
*****************************************************************************/
/**
 * @brief Pre-establish named connections.
 * @param name
 * @param url
 */
void MessageMux::registerMux(const std::string& name, const std::string& url) {
  MuxMapConstIterator iter = multiplexers().find(name);
  if ( iter == multiplexers().end() ) {
    std::pair<MuxMapIterator,bool> result;
    result = multiplexers().insert(std::pair<std::string, std::string>(name, url));
  }
}
/**
 * @brief Create a publisher for a specified multiplexer.
 *
 * As a convenience, you can optionally initialise a multiplexer for the specified url
 * at this name if it doesn't already exist. This saves from having to call
 * `registerMux` in advance.
 *
 * Note, the guts of this is just creating a socket at the mutliplexer url
 * which can do nanomsg style publishing.
 *
 * @param name
 * @param url
 * @return
 */
int MessageMux::createPublisher(const std::string& name, const std::string& url) {
  std::cout << "Creating publisher" << std::endl;
  if ( !url.empty()) {
    registerMux(name, url);
  }
  MuxMapConstIterator iter = multiplexers().find(name);
  if ( iter != multiplexers().end() ) {
    int sock = nn_socket (AF_SP, NN_PUB);
    std::cout << "Socket: " << sock << std::endl;
    // TODO check >= 0, what does 0 mean?
    nn_setsockopt (sock, NN_SUB, NN_SOCKET_NAME, name.c_str(), name.size());
    std::string url = iter->second;
    std::cout << "URL: " << url << std::endl;
    int result = nn_bind(sock, url.c_str());
    // TODO check the result
    return sock;
  } else {
    return MessageMux::DanglingConnection;
  }
}

/**
 * @brief Create a named subscriber.
 *
 * As a convenience, you can optionally establish the url for the publisher here
 * to save from having to call `registerMux` in advance.
 * @param name
 * @param url
 * @return
 */
int MessageMux::createSubscriber(const std::string& name, const std::string& url) {
  if ( !url.empty()) {
    registerMux(name, url);
  }
  MuxMapConstIterator iter = multiplexers().find(name);
  if ( iter != multiplexers().end() ) {
    int sock = nn_socket (AF_SP, NN_SUB);
    std::cout << "Socket: " << sock << std::endl;
    // TODO check >= 0, what does 0 mean?
    nn_setsockopt (sock, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
    nn_setsockopt (sock, NN_SUB, NN_SOCKET_NAME, name.c_str(), name.size());
    std::string url = iter->second;
    std::cout << "URL: " << url << std::endl;
    int result = nn_connect(sock, url.c_str());
    // TODO check the result
    return sock;
  } else {
    return MessageMux::DanglingConnection;
  }
}


MessageMux::MuxMap& MessageMux::multiplexers() {
  static MessageMux::MuxMap topic_relations;  // human consumable string - url relations
  return topic_relations;
}

} // dslam
