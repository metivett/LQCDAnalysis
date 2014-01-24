/*
 * io_manifest.hpp
 *
 *  Created on: Jun 03, 2013
 *      Author: Thibaut Metivet
 */

#ifndef IO_MANIFEST_HPP_
#define IO_MANIFEST_HPP_

#include "io.hpp"
#include <boost/filesystem.hpp>

namespace LQCDA {

    namespace {
	using namespace boost::filesystem;
    }

    class ManifestFileReader : public AsciiFileReader
    {
    private:
	path _ParentPath;

    public:
	explicit ManifestFileReader(const std::string& manifest) :
	    AsciiFileReader(manifest),
	    _ParentPath(path(manifest).parent_path())
	    {}

	template<class Op>
	Op ForEach(Op f);

    };

    template<class Op>
    Op ManifestFileReader::ForEach(Op f)
    {
	std::string sample;
	while(readLine(sample)) {
	    path p(sample);
	    if(p.is_absolute())
		f(p.native());
	    else
		f((_ParentPath / p).native());
	}
    }

} // namespace LQCDA

#endif	// IO_MANIFEST_HPP_
