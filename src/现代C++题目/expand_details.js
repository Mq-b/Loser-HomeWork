{
    window, PagedConfig = window.PagedConfig || {};
    window.PagedConfig.before = window.PagedConfig.before || function() {};
    let originalBefore = window.PagedConfig.before;
    window.PagedConfig.before = () => {
        // See https://github.com/pagedjs/pagedjs/issues/45
        originalBefore.apply(this);

        $('details').each((e) => {
            $(e).attr('open', true);
        });
    }
}
