#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test setting the attribute on xpath query results
define result <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="foo.com" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="a.cpp" hash="aa2a72b26cf958d8718a2e9bc6b84679a81d54cb"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
	</unit>

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="b.cpp" hash="520b48acbdb61e411641fd94359a82686d5591eb"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>
	</unit>

	</unit>
	STDOUT

createfile a.cpp "a;
"

createfile b.cpp "b;
"


src2srcml a.cpp b.cpp --xmlns:foo=foo.com -o archive.xml

# from a file
src2srcml archive.xml --xpath="//src:name" --element="foo:foo"
check 3<<< "$result"

src2srcml archive.xml --element="foo:foo" --xpath="//src:name"
check 3<<< "$result"

src2srcml --element="foo:foo" --xpath="//src:name" archive.xml
check 3<<< "$result"

src2srcml --xpath="//src:name" archive.xml --element="foo:foo"
check 3<<< "$result"

# output to a file
src2srcml archive.xml --xpath="//src:name" --element="foo:foo" -o result.xml
check result.xml <<< "$result"

src2srcml archive.xml --element="foo:foo" --xpath="//src:name" -o result.xml
check result.xml <<< "$result"

src2srcml --element="foo:foo" --xpath="//src:name" archive.xml -o result.xml
check result.xml <<< "$result"

src2srcml --xpath="//src:name" archive.xml --element="foo:foo" -o result.xml
check result.xml <<< "$result"

