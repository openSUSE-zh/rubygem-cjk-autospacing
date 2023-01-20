$:.push File.expand_path("lib", __dir__)

Gem::Specification.new do |s|
  s.name = "cjk-autospacing"
  s.version = "1.0.0"
  s.platform = Gem::Platform::RUBY
  s.authors = ["Marguerite Su", "Shenlebantongying"]
  s.email = ["marguerite@opensuse.org", "shenlebantongying@gmail.com"]
  s.homepage = "https://github.com/openSUSE-zh/rubygem-cjk-autospacing"
  s.summary = "Automatically add spacing between CJK and others with MarkDown syntax concerned"
  s.description = "A rubygem that automatically add sapce(U+0020) between CJK and other unicode code points, for better reading experience, with Markdown syntax concerned."
  s.files = Dir["lib/*.rb", "ext/**/*.c"] + %w[README.md Rakefile]
  s.require_paths = ["lib"]
  s.license = "MIT"
  s.extensions = ["ext/autospacing/extconf.rb"]
end
