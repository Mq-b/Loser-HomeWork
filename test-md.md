# 标题

1. 选择正确的一个选项 下面展示的代码的其行为是

```cpp
import std;
using namespace std;
int main()
{
    println("hello, world");
    return 0;
}
```

<!-- The choices -->
<div class="choices" id="choices1">
    <div class="choice" id="choice1-1">
        <input type="radio" name="choice1" value="1">A. 无法编译/未定义行为</input>
    </div>
    <div class="choice" id="choice1-2">
        <input type="radio" name="choice1" value="2">B. 可编译，保证无输出</input>
    </div>
    <div class="choice" id="choice1-3">
        <input type="radio" name="choice1" value="3">C. 可编译，保证输出"hello, world"</input>
    </div>
    <div class="choice" id="choice1-4">
        <input type="radio" name="choice1" value="4">D. 可编译，输出为实现定义</input>
    </div>

<!-- The submit button -->
<div id="submit">
    <input type="submit" value="Submit">
</div>

<script lang="javascript">
    const answers = ["3"];
    // get the choice
    function getChoice(choice) {
        for (var i = 0; i < choice.length; i++) {
            if (choice[i].checked) {
                return choice[i].value;
            }
        }
        // unchecked
        return -1;
    }

    // check answer
    function checkAnswer(question, choice) {
        // correct choice get green, wrong choice get red and show right choice green
        var choice = document.getElementsByName(choice);
        var choiceId = document.getElementById(choice);
        var choiceValue = getChoice(choice);
        // if unchecked
        if (choiceValue == -1) {
            return;
        }
        if (choiceValue == answers[question - 1]) {
            document.getElementById("choice" + question + "-" + choiceValue).style.backgroundColor = "green";
        } else {
            document.getElementById("choice" + question + "-" + choiceValue).style.backgroundColor = "red";
            document.getElementById("choice" + question + "-" + answers[question - 1]).style.backgroundColor = "green";
        }
    }

    // submit
    document.getElementById("submit").onclick = function() {
        for (var i = 1; i <= answers.length; i++) {
            checkAnswer(i, "choice" + i);
        }
    }
</script>
