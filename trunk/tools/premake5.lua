--[[
/******************************************************************************/
/*!
 \file   premake5.lua
 \author Jack Chang
 \par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
 \brief
 */
/******************************************************************************/
]]
solution "K_Mean"
  configurations { "Debug", "Release" }
  location "../"

project "K_Mean"
  targetname "K_Mean"
  location "../"
  language "C++"
  objdir "../obj"
  kind "ConsoleApp"

  -- source files
  includedirs
  {
    "../include",
    "../source",
    "../source/*/",
    "../dependency/include"
  }
  files
  {
    "../source/**.h",
    "../source/**.cpp",
    "../source/**.hpp",
    "../assets/**"
  }
  libdirs
  {
    "../dependency/lib"
  }

  configuration "Debug"
    flags { "Symbols" }
    defines { "_DEBUG", "_CRT_SECURE_NO_DEPRECATE" }
    links
    {
    }
    targetdir "../bin/debug"
    postbuildcommands
    {
      "Xcopy /S /Q /Y assets bin\\debug\\assets\\"
    }

  configuration "Release"
    flags { "Symbols" }
    optimize "Full"
    defines { "NDEBUG", "_CRT_SECURE_NO_DEPRECATE" }
    links
    {
    }
    targetdir "../bin/release"
    postbuildcommands
    {
      "Xcopy /S /Q /Y assets bin\\release\\assets\\"
    }
