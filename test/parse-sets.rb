expression = /
  (?<tetris_board>
    (?:\|[.X]{10}\|\n)+
  ){0}

  (?<tetris_piece>
    (?:[\ X]+\n)+
  ){0}

  (?<weight_value>
    -?\d+->-?\d+\.\d+
  ){0}

  (?<weight_list>
    (?:\g<weight_value>\ )*\n
  ){0}

  (?<feature_value>
    -?\d+->-?\d+\.\d+
  ){0}

  (?<feature_list>
    Features:\ (?:\g<feature_value>\ )* =>\ Value:\ -?\d+\.\d+\n
  ){0}

  Current\ piece:\n
  \g<tetris_piece>
  Wall\ height:\ \d+\n
  \g<tetris_board>
  Game\ score:\ \d+\n
  (?:\d+\ \d+\n)?
  \g<weight_list>
  \g<feature_list>
/ix

def translate_board(board)
  board.lines.map { |line|
    line.gsub(/[.X]/, {"." => " ", "X" => "#"})
  } + ["------------"]
end

data = File.read(ARGV.shift)

illegal_features = %w(
  --f-landing-height
  --f-eroded-piece-cells
)

data.scan(expression).each_with_index do |match, index|
  File.open("cases/bcts-#{index}.dat", "w") do |f|
    f.puts translate_board(match[0])

    feature_expression = /(?<=->)-?\d+\.\d+/

    match[5].scan(feature_expression).each_with_index do |value, index|
      f.puts "#{ARGV[index]} #{value}" unless illegal_features.include?(ARGV[index])
    end
  end
end