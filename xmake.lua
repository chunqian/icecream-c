set_project("log")

set_version("0.10", {build = "%Y%m%d%H%M"})

set_xmakever("2.5.3")

set_config("buildir", "xmake.Build")
set_config("mode", "debug")
set_config("plat", "macosx")
set_config("arch", "x86_64")
set_config("cxflags", "-O2")

target("test")
    
    set_kind("binary")

    set_targetdir("$(buildir)/$(mode)")

    add_defines("LOG_USE_COLOR")

    add_files("test/test.c")

    add_includedirs(
        "."
        )

    add_cflags("-std=c99")

    after_build(function(target)
        import("core.base.task")
        task.run("project", {kind = "compile_commands"})
    end)
