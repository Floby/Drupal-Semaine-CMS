<?php // $Id: views-view-fluidgrid.tpl.php,v 1.1.2.2 2010/03/16 07:16:01 doublethink Exp $ ?>
<?php print $script ?>
<div id="<?php print $views_fluidgrid_id ?>" class="views-fluidgrid-wrapper clear-block">
    <?php foreach ($rows as $row): ?>
      <div class="<?php print $views_fluidgrid_item_class ?>">
        <div class="views-fluidgrid-item-inner">
          <?php print $row ?>
        </div>
      </div>
    <?php endforeach; ?>
</div>