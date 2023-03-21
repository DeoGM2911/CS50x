function altBgImg() {
    document.querySelector("#nav-button").addEventListener("click", function(){
        document.querySelector("#main-nav").style.backgroundImage = "url('qface.webp')";
        altBgImgTemp()
        });
}
      
function altBgImgTemp(url2) {
    document.querySelector("#nav-button").addEventListener("click", function(){
        document.querySelector("#main-nav").style.backgroundImage = "url('rcircle.png')";
        altBgImg();
    });
}