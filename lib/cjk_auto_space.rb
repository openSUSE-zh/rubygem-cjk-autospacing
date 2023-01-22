begin
  ruby_version = /(\d+\.\d+)/.match(::RUBY_VERSION)
  require_relative "cjk_auto_space/#{ruby_version}/cjk_auto_space"
rescue LoadError
  require "cjk_auto_space/cjk_auto_space"
end

String.class_eval do
  def cjk_auto_space!
    sub!(self, self.cjk_auto_space)
  end
end
