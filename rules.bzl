# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

def gla_lib(name, header_only, minimum_profile, maximum_profile, extensions = [], **kwargs):
    native.genrule(
        name = "%s_source_generator" % name,
        srcs = [
            "@gla//:specification_files",
            "@gla//:templates/include/GLA/gla.h",
            "@gla//:templates/src/GLA/gla.c",
        ],
        outs = ["%s/include/GLA/gla.h" % name] + ([] if header_only else ["%s/src/GLA/gla.c" % name]),
        cmd =
            "$(location @gla//:gla_generator) --quiet %s --minimum_profile %s --maximum_profile %s --output_directory $(RULEDIR)/%s %s" % (
                "--header_only" if header_only else "",
                minimum_profile,
                maximum_profile,
                name,
                " ".join(["--extension " + e for e in extensions]),
            ),
        tools = ["@gla//:gla_generator"],
    )

    native.cc_library(
        name = name,
        srcs = [] if header_only else ["%s/src/GLA/gla.c" % name],
        hdrs = [
            "%s/include/GLA/gla.h" % name,
            "@gla//:specification_files",
        ],
        includes = ["%s/include" % name],
        deps = ["@gla//:gl"],
        **kwargs
    )
