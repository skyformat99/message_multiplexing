/**
 * @file /mm_vision_msgs/include/mm_vision_msgs/image.hpp
 * 
 * @brief Convert to our simple image format.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_vision_msgs_IMAGE_HPP_
#define mm_vision_msgs_IMAGE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

//#include <ecl/utilities/parameters.hpp>
#include <ecl/time/timestamp.hpp>
#include <mm_messages/message.hpp>
#include <memory>
#include <vector>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_vision_msgs {

/*****************************************************************************
** Image Definitions
*****************************************************************************/
/**
 * @brief An image structure that makes sense for non-opencv platforms.
 *
 * On our embedded boards we usually dont run with opencv, but we need a
 * structure which we can fling around. This is it.
 */
struct Image {
  std::vector<unsigned char> data;
  bool colour;
  unsigned int width, height, size; // calculate from incoming buffer sizes
  // use this as a semantic identifer for this image - doesn't need to be defined
  // here, use your own id/enum list to identify it within your programs.
  unsigned int semantic_id;
  ecl::TimeStamp timestamp;
};

typedef std::shared_ptr<Image> ImagePtr;

} // namespace mm_vision_msgs

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
** Serialisation
*****************************************************************************/

template<>
struct Message<mm_vision_msgs::ImagePtr> {
  static void encode(const mm_vision_msgs::ImagePtr& s, ByteArray& buffer);
  static mm_vision_msgs::ImagePtr decode(const unsigned char* buffer, const unsigned int& size);
};

} // namespace mm_messages

#endif /* mm_vision_msgs_IMAGE_HPP_ */