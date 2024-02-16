{
    window, PagedConfig = window.PagedConfig || {};
    window.PagedConfig.before = window.PagedConfig.before || function () { };
    let originalBefore = window.PagedConfig.before;
    window.PagedConfig.before = () => {
        originalBefore.apply(this);

        $('details').each(function () {
            $(this).wrapInner('<div></div>');
            $(this).children().unwrap();
        }).remove();
        $('summary').remove();
    }
}
