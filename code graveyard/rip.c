int backtrack_engine(bool zeroed, bool stack_empty, map *got_map, stack *got_stack, bridge *last_point, list *isla_list, int mode)
{
    node   *aux_node;
    bridge *aux_bridge;
    isla   *try_isla;
    list   *probi_list;

    printf("Last Point: %d-%d \n", get_isla_name(get_points(last_point, 0)), get_isla_name(get_points(last_point, 1)));
    printf("To remove : %d-%d \n", get_isla_name(get_points(get_node_item(get_head(got_stack)), 0)), get_isla_name(get_points(get_node_item(get_head(got_stack)), 1)));
    printf("Trying to backtack. Last stack \n");
    print_stack(got_stack, print_bridge);
    /* If we already determined it is zeroed */
    if(zeroed == TRUE)
    {
        return GOT_SOL;
    }

    if(stack_empty == TRUE)
    {
        return NO_SOL;
    }

    /* Free prohibition list from head bridge */
    free_connected_nodes(get_head(get_bridge_probi_list(get_node_item(get_stack_head(got_stack)))), free_bridge);
    /* Push head to prohibited list of head->next */
    push_item_to_list(get_bridge_probi_list(last_point), get_node_item(get_stack_head(got_stack)));

    /* Bummer, that did not work. Get over depression and get on re(cursing) */
    /* Pop stack until last point */
    while( (bridge *)get_node_item(get_stack_head(got_stack)) != last_point )
    {
        aux_node = pop_from_stack(got_stack); /* Pop node from stack */
        aux_bridge = (bridge *) get_node_item(aux_node);

        remove_bridge(aux_bridge);
        free_node(aux_node, already_free);
    }

    probi_list = get_bridge_probi_list(get_node_item(get_stack_head(got_stack)));

    try_isla = get_isla_for_dfs(isla_list);
    DFS_ignition(got_stack, try_isla, got_map, isla_list, probi_list, mode);

    /* That did not check out, so let us check for all zero on map*/
    if(check_for_allzero(isla_list) == TRUE)
    {
        zeroed = TRUE;
        free_connected_nodes(get_head(get_bridge_probi_list(last_point)), free_bridge);
        return GOT_SOL;
    }

    if(get_next_node(get_stack_head(got_stack)) == NULL)
    {
        free_connected_nodes(get_head(get_bridge_probi_list(last_point)), free_bridge);
        stack_empty = TRUE;
        return NO_SOL;
    }

    backtrack_engine(zeroed, stack_empty, got_map, got_stack, get_node_item(get_next_node(get_stack_head(got_stack))), isla_list, mode);

    return 0;
}
