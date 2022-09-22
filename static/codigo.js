$(document).ready(function () {
    var table = $('#example').DataTable({
        pagingType: 'full_numbers',
        searching: true,
    });

    $('#form').on('submit',function(e){
        e.preventDefault();

        $.ajax({
            type: "POST",
            url: $(this).attr('action'),
            data: {key1:$("#dataset1").val(),key2:$("#dataset2").val(),key3:$("#exampleFormControlTextarea1")},
            dataType: "json"
        });
    });
    
    $('#dataset1').click(function(){
        $('#dataset1').removeClass('btn-light')
        $('#dataset1').val("True")
        $('#dataset2').val("False")

        $('#dataset1').toggleClass('btn-primary')
        $('#dataset2').removeClass('btn-primary')
        $('#dataset2').toggleClass('btn-light')
    });

    $('#dataset2').click(function(){
        $('#dataset2').removeClass('btn-light')
        $('#dataset2').val("True")
        $('#dataset1').val("False")
        $('#dataset2').toggleClass('btn-primary')
        $('#dataset1').removeClass('btn-primary')
        $('#dataset1').addClass('btn-light')
    });
});


