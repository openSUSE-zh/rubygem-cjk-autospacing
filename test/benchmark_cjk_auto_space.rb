#!/usr/bin/env ruby

require 'benchmark'
require 'autocorrect-rb'
require 'cjk_auto_space'

arr = []
open("titles.txt","r").each_line do |line|
  arr << line
end

Benchmark.bm do |benchmark|
  benchmark.report("autocorrect-rb") do
    arr.each do |str|
      AutoCorrect.format(str)
    end
  end

  benchmark.report("cjk_auto_space") do
    arr.each do |str|
      str.cjk_auto_space!
    end
  end
end
