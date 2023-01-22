require "rake/extensiontask"
require "rake/testtask"

Rake::ExtensionTask.new "cjk_auto_space" do |ext|
  ext.lib_dir = "lib/cjk_auto_space"
end

Rake::TestTask.new do |t|
  t.libs << "test"
end

desc "Run tests"
task default::test
