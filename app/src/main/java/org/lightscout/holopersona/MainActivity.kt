package org.lightscout.holopersona

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Person
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.WindowInsetsControllerCompat
import org.lightscout.holopersona.ui.theme.HoloPersonaTheme

class MainActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        enableEdgeToEdge()

        setContent { HoloPersonaTheme { HoloPersonaApp() } }
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        if (hasFocus) {
            hideSystemUi()
        }
    }

    private fun hideSystemUi() {
        WindowCompat.setDecorFitsSystemWindows(window, false)
        val insetsController = WindowInsetsControllerCompat(window, window.decorView)

        // Hide system bars
        insetsController.hide(WindowInsetsCompat.Type.systemBars())

        // Set immersive sticky behavior
        insetsController.systemBarsBehavior =
                WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun HoloPersonaApp() {
    var selectedSkeletonType by remember { mutableIntStateOf(1) } // DETAILED_HUMANOID
    var showControls by remember { mutableStateOf(true) }

    Box(modifier = Modifier.fillMaxSize()) {
        // 3D Background View
        AndroidView(
                factory = { context ->
                    HoloPersonaGLSurfaceView(context).apply {
                        setSkeletonType(selectedSkeletonType)
                    }
                },
                modifier = Modifier.fillMaxSize(),
                update = { view -> view.setSkeletonType(selectedSkeletonType) }
        )

        // UI Overlay
        if (showControls) {
            Column(
                    modifier = Modifier.fillMaxSize().padding(16.dp),
                    verticalArrangement = Arrangement.SpaceBetween
            ) {
                // Top Controls
                Card(
                        modifier = Modifier.fillMaxWidth(),
                        colors =
                                CardDefaults.cardColors(
                                        containerColor =
                                                MaterialTheme.colorScheme.surface.copy(alpha = 0.9f)
                                )
                ) {
                    Column(modifier = Modifier.padding(16.dp)) {
                        Text(
                                text = "HoloPerson",
                                style = MaterialTheme.typography.headlineSmall,
                                color = MaterialTheme.colorScheme.onSurface
                        )
                        Spacer(modifier = Modifier.height(8.dp))
                        Text(
                                text = "3D Human Figure with Jetpack Compose",
                                style = MaterialTheme.typography.bodyMedium,
                                color = MaterialTheme.colorScheme.onSurfaceVariant
                        )
                    }
                }

                // Bottom Controls
                Card(
                        modifier = Modifier.fillMaxWidth(),
                        colors =
                                CardDefaults.cardColors(
                                        containerColor =
                                                MaterialTheme.colorScheme.surface.copy(alpha = 0.9f)
                                )
                ) {
                    Column(modifier = Modifier.padding(16.dp)) {
                        Text(
                                text = "Skeleton Type",
                                style = MaterialTheme.typography.titleMedium,
                                color = MaterialTheme.colorScheme.onSurface
                        )
                        Spacer(modifier = Modifier.height(8.dp))

                        // Skeleton Type Selector
                        Row(
                                modifier = Modifier.fillMaxWidth(),
                                horizontalArrangement = Arrangement.SpaceEvenly
                        ) {
                            val skeletonTypes =
                                    listOf(
                                            0 to "Basic",
                                            1 to "Detailed",
                                            2 to "Athletic",
                                            3 to "Slim"
                                    )

                            skeletonTypes.forEach { (type, name) ->
                                FilterChip(
                                        onClick = { selectedSkeletonType = type },
                                        label = { Text(name) },
                                        selected = selectedSkeletonType == type,
                                        leadingIcon =
                                                if (selectedSkeletonType == type) {
                                                    {
                                                        Icon(
                                                                Icons.Default.Person,
                                                                contentDescription = null
                                                        )
                                                    }
                                                } else null
                                )
                            }
                        }

                        Spacer(modifier = Modifier.height(16.dp))

                        // Hide/Show Controls Button
                        Button(
                                onClick = { showControls = false },
                                modifier = Modifier.fillMaxWidth()
                        ) { Text("Hide Controls") }
                    }
                }
            }
        }

        // Show Controls Button (when hidden)
        if (!showControls) {
            FloatingActionButton(
                    onClick = { showControls = true },
                    modifier = Modifier.align(Alignment.BottomEnd).padding(16.dp)
            ) { Icon(Icons.Default.Person, contentDescription = "Show Controls") }
        }
    }
}
