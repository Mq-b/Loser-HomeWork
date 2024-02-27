{
    window, PagedConfig = window.PagedConfig || {};
    window.PagedConfig.before = window.PagedConfig.before || function () { };
    let originalBefore = window.PagedConfig.before;
    window.PagedConfig.before = () => {
        originalBefore.apply(this);

        $('details').remove();
    }
}
