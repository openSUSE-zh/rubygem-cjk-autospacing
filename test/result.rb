#!/usr/bin/env ruby

require "cjk_auto_space"

arr = []
open("posts.txt","r").each_line do |line|
  arr << line unless line.strip.empty?
end

arr.each do |line|
  if line.match?(/[\p{Han}|\p{Hangul}|\p{Hiragana}|\p{Katakana}]+/)
    puts "raw:" + line
    puts "cooked:" + line.cjk_auto_space.force_encoding("UTF-8")
  end
end
