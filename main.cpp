#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <random>
#include <algorithm>
#include <windows.h>

struct Question {
    std::wstring text;
    std::vector<std::wstring> options;
    std::vector<wchar_t> correct;
};

std::wstring trim(const std::wstring& s) {
    size_t start = s.find_first_not_of(L" \t\r\n");
    size_t end = s.find_last_not_of(L" \t\r\n");
    if (start == std::wstring::npos || end == std::wstring::npos) return L"";
    return s.substr(start, end - start + 1);
}

std::wstring escapeHTML(const std::wstring& input) {
    std::wstring result = input;
    std::wregex amp(L"&");
    std::wregex lt(L"<");
    std::wregex gt(L">");
    result = std::regex_replace(result, amp, L"&amp;");
    result = std::regex_replace(result, lt, L"&lt;");
    result = std::regex_replace(result, gt, L"&gt;");
    return result;
}

bool isOptionLine(const std::wstring& line) {
    return line.size() >= 3 && iswalpha(line[0]) && line[1] == L'.' && line[2] == L' ';
}

std::wstring utf8_to_wstring(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
    return wstr;
}

std::string wstring_to_utf8(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed, NULL, NULL);
    return str;
}

int main() {
    std::ifstream infile("quiz_input.txt");
    std::vector<Question> questions;
    std::string line_utf8;
    std::vector<std::wstring> lines;

    while (std::getline(infile, line_utf8)) {
        std::wstring line = utf8_to_wstring(line_utf8);
        if (!trim(line).empty()) {
            lines.push_back(trim(line));
        }
    }

    std::random_device rd;
    std::mt19937 rng(rd());

    for (size_t i = 0; i + 1 < lines.size(); ++i) {
        // Detect a question line
        if (lines[i].rfind(L"Câu hỏi:", 0) == 0 || !isOptionLine(lines[i])) {
            Question q;
            if (lines[i].rfind(L"Câu hỏi:", 0) == 0) {
                q.text = trim(lines[i].substr(8)); // Remove "Câu hỏi:"
            }
            else {
                q.text = trim(lines[i]);           // Use line directly as question
            }

            std::map<std::wstring, int> option_map;
            std::vector<std::wstring> options_raw;
            size_t j = i + 1;

            // Collect options until next question line or break
            for (; j < lines.size(); ++j) {
                if (lines[j].rfind(L"Câu hỏi:", 0) == 0 || !isOptionLine(lines[j])) {
                    break; // next question detected
                }
                if (isOptionLine(lines[j])) {
                    option_map[lines[j]]++;
                    options_raw.push_back(lines[j]);
                }
            }

            // Remove duplicates
            std::vector<std::wstring> unique_options;
            std::map<wchar_t, std::wstring> letter_to_option;
            for (const auto& opt : options_raw) {
                wchar_t label = opt[0];
                if (letter_to_option.count(label) == 0) {
                    letter_to_option[label] = opt;
                    unique_options.push_back(opt);
                }
            }

            // Shuffle options
            std::shuffle(unique_options.begin(), unique_options.end(), rng);
            q.options = unique_options;

            // Mark correct answers (duplicates)
            for (const auto& [opt, count] : option_map) {
                if (count > 1 && iswalpha(opt[0])) {
                    q.correct.push_back(opt[0]);
                }
            }

            questions.push_back(q);
            i = j - 1; // continue after this question
        }
    }


    std::ofstream outfile("quiz.html");
    outfile << "<!DOCTYPE html><html lang=\"vi\"><head><meta charset=\"UTF-8\">\n";
    outfile << "<title>Quiz</title><style>.correct{color:green}.incorrect{color:red}</style></head><body>\n";
    outfile << "<h1>Bài kiểm tra pháp luật</h1><form id=\"quizForm\">\n";

    for (size_t i = 0; i < questions.size(); ++i) {
        outfile << "<div class=\"question\"><p><strong>Câu " << (i + 1) << ": "
            << wstring_to_utf8(escapeHTML(questions[i].text)) << "</strong></p>\n";
        for (const auto& opt : questions[i].options) {
            wchar_t label = opt[0];
            outfile << "<label><input type=\"checkbox\" name=\"q" << i << "\" value=\"" << (char)label << "\"> "
                << wstring_to_utf8(escapeHTML(opt)) << "</label><br>\n";
        }
        outfile << "<div id=\"result-q" << i << "\"></div></div>\n";
    }

    outfile << "<button type=\"submit\">Nộp bài</button></form>\n<script>\n";
    outfile << "const correctAnswers = {\n";
    for (size_t i = 0; i < questions.size(); ++i) {
        outfile << "  q" << i << ": [";
        for (size_t j = 0; j < questions[i].correct.size(); ++j) {
            outfile << "'" << (char)questions[i].correct[j] << "'";
            if (j + 1 < questions[i].correct.size()) outfile << ", ";
        }
        outfile << "]";
        if (i + 1 < questions.size()) outfile << ",";
        outfile << "\n";
    }
    outfile << "};\n";

    outfile << R"(
document.getElementById('quizForm').addEventListener('submit', function(e) {
  e.preventDefault();
  let score = 0;
  let total = Object.keys(correctAnswers).length;
  for (const [key, correct] of Object.entries(correctAnswers)) {
    const selected = Array.from(document.querySelectorAll(`input[name="${key}"]:checked`)).map(el => el.value);
    const resultEl = document.getElementById(`result-${key}`);
    const correctSet = new Set(correct);
    const selectedSet = new Set(selected);
    const isCorrect = correct.length === selected.length && correct.every(c => selectedSet.has(c));
    if (isCorrect) {
      resultEl.textContent = 'Đúng';
      resultEl.className = 'correct';
      score++;
    } else {
      resultEl.textContent = `Wrong (Correct answer: ${correct.join(', ')})`;
      resultEl.className = 'incorrect';
    }
  }
  let point = score/total * 10;
  alert(`Correct answer : ${score} / ${total}. Points : ${point}`);
});
    )";

    outfile << "</script></body></html>\n";

    std::cout << "Successfully created " << questions.size() << " questions!";
    return 0;
}
