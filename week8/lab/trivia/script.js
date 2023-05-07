document.addEventListener("DOMContentLoaded", function () {
  let res1 = document.querySelector("#res1");
  let res2 = document.querySelector("#res2");

  // Listen for correct answer
  let correctButton = document.querySelector(".correct");
  correctButton.addEventListener("click", function () {
    correctButton.style.background = "green";
    res1.innerHTML = "Correct!";
    res1.style.color = "green";
    btn.style.background = "green";
  });

  // Listen for incorrect answers
  let incorrects = document.querySelectorAll(".incorrect");
  for (let index = 0; index < incorrects.length; index++) {
    const btn = incorrects[index];
    btn.addEventListener("click", function () {
      btn.style.background = "red";
      res1.innerHTML = "Incorrect!";
      res1.style.color = "red";
    });
  }

  // Listen for the fre answer
  document.querySelector(".sm").addEventListener("click", function () {
    let box = document.querySelector("#ans");
    let ans = box.value;
    if (ans == "Russia") {
      res2.innerText = "Correct!";
      res2.style.color = "green";
      box.style.backgroundColor = "green";
    } else {
      res2.innerText = "Incorrect!";
      res2.style.color = "red";
      box.style.backgroundColor = "red";
    }
  });
});
