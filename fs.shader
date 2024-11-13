#version 460

in vec3 position_eye, normal_eye;
uniform mat4 view_mat_shader;

in vec4 fragment_color;
out vec4 out_color;

// Các biến uniform để bật/tắt từng nguồn sáng
uniform bool enable_light_1; // Bật/tắt nguồn sáng 1
uniform bool enable_light_2; // Bật/tắt nguồn sáng 2
uniform int color;  // Nhận giá trị từ mã C++ cho color

void main() {
    vec3 final_color = vec3(0);

    // Kiểm tra xem có sử dụng màu xanh bầu trời hay không
    if (color == 16) {
        final_color = vec3(0.5294117647, 0.8078431373, 0.9215686275); // Màu skyblue
    } else {
        // Nếu không phải là màu skyblue, tiếp tục với việc tính toán ánh sáng
        //-----------Nguồn sáng 1-------------
        if (enable_light_1) {
            vec3 light_position_world_1 = vec3(0.0, 2.27, -3.0);
            vec3 light_position_eye_1 = vec3(view_mat_shader * vec4(light_position_world_1, 1.0));
            vec3 position_viewer = vec3(0.0, 0.0, 0.0);
            vec3 n_eye = normalize(normal_eye);

            vec3 distance_to_light_eye_1 = light_position_eye_1 - position_eye;
            vec3 direction_to_light_eye_1 = normalize(distance_to_light_eye_1);
            vec3 surface_to_viewer_eye_1 = normalize(position_viewer - position_eye);

            float dot_prod_1 = max(dot(direction_to_light_eye_1, n_eye), 0.0);
            vec3 Ld_1 = vec3(0.7, 0.7, 0.7);
            vec3 Kd_1 = vec3(1.0, 1.0, 1.0);
            vec3 Id_1 = Ld_1 * Kd_1 * dot_prod_1;

            vec3 half_way_eye_1 = normalize(surface_to_viewer_eye_1 + direction_to_light_eye_1);
            float dot_prod_specular_1 = max(dot(half_way_eye_1, n_eye), 0.0);
            float specular_factor_1 = pow(dot_prod_specular_1, 20.0);
            vec3 Ls_1 = vec3(0.2, 0.2, 0.2);
            vec3 Ks_1 = vec3(1.0, 1.0, 1.0);
            vec3 Is_1 = Ls_1 * Ks_1 * specular_factor_1;

            vec3 La_1 = vec3(0.2, 0.2, 0.2);
            vec3 Ka_1 = vec3(1.0, 1.0, 1.0);
            vec3 Ia_1 = La_1 * Ka_1;

            final_color += (Is_1 + Id_1 + Ia_1);
        }

        //-----------Nguồn sáng 2-------------
        if (enable_light_2) {
            vec3 light_position_world_2 = vec3(2.0, 15.0, 2.0);
            vec3 light_position_eye_2 = vec3(view_mat_shader * vec4(light_position_world_2, 1.0));
            vec3 position_viewer = vec3(0.0, 0.0, 0.0);
            vec3 n_eye = normalize(normal_eye);

            vec3 distance_to_light_eye_2 = light_position_eye_2 - position_eye;
            vec3 direction_to_light_eye_2 = normalize(distance_to_light_eye_2);
            vec3 surface_to_viewer_eye_2 = normalize(position_viewer - position_eye);

            float dot_prod_2 = max(dot(direction_to_light_eye_2, n_eye), 0.0);
            vec3 Ld_2 = vec3(0.7, 0.7, 0.7);
            vec3 Kd_2 = vec3(1, 0.5, 0);
            vec3 Id_2 = Ld_2 * Kd_2 * dot_prod_2;

            vec3 half_way_eye_2 = normalize(surface_to_viewer_eye_2 + direction_to_light_eye_2);
            float dot_prod_specular_2 = max(dot(half_way_eye_2, n_eye), 0.0);
            float specular_factor_2 = pow(dot_prod_specular_2, 50.0);
            vec3 Ls_2 = vec3(0.8, 0.8, 1.0);
            vec3 Ks_2 = vec3(0.7, 0.7, 0.7);
            vec3 Is_2 = Ls_2 * Ks_2 * specular_factor_2;

            vec3 La_2 = vec3(0.1, 0.1, 0.3);
            vec3 Ka_2 = vec3(0.5, 0.5, 0.5);
            vec3 Ia_2 = La_2 * Ka_2;

            final_color += (Is_2 + Id_2 + Ia_2);
        }
    }

    final_color = final_color * fragment_color.rgb; // Kết hợp với màu nền từ fragment_color

    out_color = vec4(final_color, 1.0);
}
