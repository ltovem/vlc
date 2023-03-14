--[==========================================================================[
 luac.lua: lua compilation module for VLC (duplicates luac)
--[==========================================================================[
 Copyright (C) 2010 Antoine Cellerier

 Authors: Antoine Cellerier <dionoea at videolan dot org>

 SPDX-License-Identifier: GPL-2.0-or-later
--]==========================================================================]

usage =
[[
To compile a lua script to bytecode (luac) run:
  vlc -I luaintf --lua-intf luac --lua-config 'luac={input="file.lua",output="file.luac"}'
Output will be similar to that of the luac command line tool provided with lua with the following arguments:
  luac -o file.luac file.lua
]]

function compile()
    vlc.msg.info("About to compile lua file")
    vlc.msg.info("  Input is '"..tostring(config.input).."'")
    vlc.msg.info("  Output is '"..tostring(config.output).."'")
    if not config.input or not config.output then
        vlc.msg.err("Input and output config options must be set")
        return false
    end

    local bytecode, msg = loadfile(config.input)
    if not bytecode then
        vlc.msg.err("Error while loading file '"..config.input.."': "..msg)
        return false
    end

    local output, msg = io.open(config.output, "wb")
    if not output then
        vlc.msg.err("Error while opening file '"..config.output.."' for output: "..msg)
        return false
    else
        output:write(string.dump(bytecode))
        return true
    end
end


if not compile() then
    for line in string.gmatch(usage,"([^\n]+)\n*") do
        vlc.msg.err(line)
    end
end
vlc.misc.quit()

