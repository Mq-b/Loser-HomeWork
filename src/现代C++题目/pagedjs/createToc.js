// making it a hook to run inside paged.js and not before

function createToc(config) {
    const content = config.content;
    const tocElement = config.tocElement;
    const titleElements = config.titleElements;

    let tocElementDiv = content.querySelector(tocElement);
    if (!tocElementDiv) return console.warn("couldn’t start the toc");
    tocElementDiv.innerHTML = "";
    let tocUl = document.createElement("ul");
    tocUl.id = "list-toc-generated";
    tocElementDiv.appendChild(tocUl);

    // add class to all title elements
    let tocElementNbr = 0;
    for (var i = 0; i < titleElements.length; i++) {
        let titleHierarchy = i + 1;
        let titleElement = content.querySelectorAll(titleElements[i]);

        titleElement.forEach(function (element) {
            // check if shouldbe shown
            if (
                !(
                    element.closest("section").classList.contains("toc-ignore") ||
                    element.closest("section").classList.contains("toc")
                )
            ) {
                // add classes to the element
                element.classList.add("title-element");
                element.setAttribute("data-title-level", titleHierarchy);

                // add an id if doesn't exist
                tocElementNbr++;

                if (element.id == "") {
                    element.id = "title-element-" + tocElementNbr;
                }
                let newIdElement = element.id;
            }
        });
    }

    // create toc list
    let tocElements = content.querySelectorAll(".title-element");

    for (var i = 0; i < tocElements.length; i++) {
        let tocElement = tocElements[i];

        let tocNewLi = document.createElement("li");

        // Add class for the hierarcy of toc
        tocNewLi.classList.add("toc-element");
        tocNewLi.classList.add(
            "toc-element-level-" + tocElement.dataset.titleLevel
        );

        let classes = [
            ...tocElement.className.split(" "),
            ...tocElement.closest("section")?.className.split(" "),
        ];

        classes.forEach((meta) => {
            if (meta == "title-element" || meta == undefined || meta == "") return;
            tocNewLi.classList.add(`toc-${meta}`);
        });

        //get the exisiting class
        // Keep class of title elements
        let classTocElement = tocElement.classList;
        for (var n = 0; n < classTocElement.length; n++) {
            if (classTocElement[n] != "title-element") {
                tocNewLi.classList.add(classTocElement[n]);
            }
        }

        // Create the element
        tocNewLi.innerHTML =
            '<a href="#' + tocElement.id + '">' + tocElement.textContent + "</a>";
        tocUl.appendChild(tocNewLi);
    }
}
