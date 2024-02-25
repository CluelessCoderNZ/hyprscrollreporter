# Hyprscrollreporter

+ Simple utility plugin extends hyprland to output its current workspace animation offset via IPC. Animation offsets are
the sliding animation you see when transitioning between workspaces, also for touchpad gestures. 
+ By exporting this value you can allow other utilities to track alongside the animation in time. In particular a scrolling wallpaper I am developing.

## Event `workspaceoffset`
Event data members in order:
+ `monitor_name`: Monitor name of workspace host.
+ `monitor_id`: Monitor id workspace host.
+ `active_workspace`: Id of active workspace.
+ `previous_workspace`: Id of previous workspace.
+ `unit_offset_x`: [-1, +1] value of transition across monitor width.
+ `unit_offset_y`: [-1, +1] value of transition across monitor height. For vertical workspace setups.

**Example IPC Output:**
```
workspace>>2
workspaceoffset>>WL-1,0,2,1,1.000000,0.000000
workspaceoffset>>WL-1,0,2,1,0.825364,0.000000
workspaceoffset>>WL-1,0,2,1,0.552560,0.000000
```
