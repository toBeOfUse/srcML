/**
 * @file src_input_remote.hpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
  src_input_remote assigns remote files and archives to the srcml parsing queue
*/

#ifndef SRC_INPUT_REMOTE_HPP
#define SRC_INPUT_REMOTE_HPP

#include <srcml.h>
#include <parse_queue.hpp>
#include <string>
#include <boost/optional.hpp>

void src_input_remote(ParseQueue& queue,
                      srcml_archive* srcml_arch,
                      const std::string& remote_uri,
                      const boost::optional<std::string>& option_language,
                      const boost::optional<std::string>& option_filename,
                      const boost::optional<std::string>& option_directory,
                      const boost::optional<std::string>& option_version);

#endif
