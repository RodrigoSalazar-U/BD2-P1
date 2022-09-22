$(document).ready(function () {
    var table = $('#example').DataTable({
        pagingType: 'full_numbers',
        searching: true,
    });

    table.ajax.reload();
    
    $('#dataset1').click(function(){
        $('#dataset1').removeClass('btn-light')

        $('#dataset1').toggleClass('btn-primary')
        $('#dataset2').removeClass('btn-primary')
        $('#dataset2').toggleClass('btn-light')
    });

    $('#dataset2').click(function(){
        $('#dataset2').removeClass('btn-light')

        $('#dataset2').toggleClass('btn-primary')
        $('#dataset1').removeClass('btn-primary')
        $('#dataset1').addClass('btn-light')
    });
});


