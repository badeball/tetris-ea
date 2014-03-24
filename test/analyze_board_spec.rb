require "minitest/autorun"
require "minitest/reporters"

Minitest::Reporters.use! Minitest::Reporters::ProgressReporter.new

class AnalyzeBoardTest < MiniTest::Unit::TestCase
  BOARD_EXPRESSION = /
    (\|[\ #]{10}\|\n){20}
    -{12}\n
  /ix

  FEATURE_EXPRESSION = /
    --f-[a-z\-]+\ -?\d+.\d+\n
  /ix

  FEATURE_NAMES = /
    --f-[a-z\-]+
  /ix

  @@executable = ENV["ANALYZE_BOARD"] || "../code/bin/analyze_board"

  raise "Unknown executable!" unless File.exist?(@@executable)

  Dir.foreach('cases') do |item|
    next if item == '.' or item == '..'

    test_name = "test_" << item.split('.').first.gsub("-", "_")

    define_method test_name do
      case_data = File.read("cases/#{item}")

      analyzation = `cat cases/#{item} | #{@@executable} #{case_data.scan(FEATURE_NAMES) * ' '}`

      case_data.scan(FEATURE_EXPRESSION).each do |feature|
        name = feature[/--f-[a-z\-]+/]
        value = feature[/-?\d+.\d/]

        assert_match /#{name}\s+#{value}/, analyzation, case_data[BOARD_EXPRESSION]
      end
    end
  end
end